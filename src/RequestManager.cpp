/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:52:23 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 20:53:34 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

RequestManager::RequestManager(Socket& socket, ConfigFile& configFile)
    :_stateMachine(RECV_HEADER, STATE_AM), _socket(socket), _configFile(configFile),
        _requestParser(_request, _response), _requestPerformer(_request, _response),
        _responseSender(_request, _response)
{
    (void)_configFile; //TODO MUST REMOVE
}
RequestManager::~RequestManager(){}

void    RequestManager::handle(void)
{
    if (_stateMachine.getCurrentState() == RECV_HEADER) {
        //I cannot be sure by this point whether I actually need to read from socket
        // to parse the request, as it can be enough leftover from a previous read to
        // parse an entire nex request. Therefore I must try to parse leftovers and
        // only then read and parse more if necessary (and possible)
        _requestParser.handle(); 
        if (!_requestParser.isDone() && _socket.canRecv()) {
            _socket.fillStash();
            _requestParser.handle();
        }
        if (_requestParser.isDone()) {
            _moveOnFromParsing();
        }
    }
    if (_stateMachine.getCurrentState() == RECV_BODY) {
        _requestPerformer.handle();
        if (!_requestPerformer.isDone() && _socket.canRecv()) {
            _socket.fillStash();
            _requestPerformer.handle();
        }
        if (_requestPerformer.isDone()) {
            _moveOnFromParsing();
        }
    }
    if (_stateMachine.getCurrentState() == SEND_RESPONSE) {
        _responseSender.handle(); 
        if (_socket.canSend()) {
            _socket.flushStash();
        }
        if (_responseSender.isDone()) {
            _setHandlingComplete(true);
        }
    }
}

bool    RequestManager::isDone(void) const {return (_getHandlingComplete());}
bool    RequestManager::shouldCloseConnection(void) const {return (_getCloseConnection());}


void    RequestManager::_setHandlingComplete(bool value) {_handlingComplete = value;}
bool    RequestManager::_getHandlingComplete(void) const {return (_handlingComplete);}
void    RequestManager::_setCloseConnection(bool value) {_closeConnection = value;}
bool    RequestManager::_getCloseConnection(void) const {return (_closeConnection);}

void    RequestManager::_moveOnFromParsing(void)
{
    ErrorSeverity   errorSeverity = _getErrorSeverity(_response.getStatusCode());
    switch (errorSeverity) {
        case NO_ERROR:
            _stateMachine.advanceState();
            break;
        case CONSUME_AND_ANSWER:
            _requestPerformer.activateConsumeMode();
            _stateMachine.advanceState();
            break;
        case ANSWER_AND_CLOSE:
            _setCloseConnection(true);
            _stateMachine.setCurrentState(SEND_RESPONSE);
            break;
        case CLOSE_IMMEDIATELY:
            _setHandlingComplete(true);
            _setCloseConnection(true);
            break;
        default:
            throw (std::exception()); //TODO explicit
    }
}

RequestManager::ErrorSeverity   RequestManager::_getErrorSeverity(code_t statusCode)
{
    switch (statusCode) {
        case 200:
            return (NO_ERROR);
        case 405:
            return (CONSUME_AND_ANSWER);
        case 404:
            return (ANSWER_AND_CLOSE);
        case -1: // This is not a real code, is an internal indication that some bad shit happened
            return (CLOSE_IMMEDIATELY);
        default:
            throw (std::exception()); //TODO explicit
    }
}