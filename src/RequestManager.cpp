/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:52:23 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/28 12:17:59 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

RequestManager::RequestManager(Socket& socket, ConfigFile& configFile)
    :_stateMachine(STATE_AM, RECV_HEADER), _socket(socket), _configFile(configFile),
        _requestParser(_request, _response, _configFile, getPortFromSocket(_socket.getSockFd())),
        _requestPerformer(_request, _response),
        _responseSender(_request, _response),
        _handlingComplete(false), _closeConnection(false)
{(void)_configFile;}
RequestManager::~RequestManager(){}

void    RequestManager::handle(void)
{
    if (_stateMachine.getCurrentState() == RECV_HEADER) {
        //I cannot be sure by this point whether I actually need to read from socket
        // to parse the request, as it can be enough leftover from a previous read to
        // parse an entire nex request. Therefore I must try to parse leftovers and
        // only then read and parse more if necessary (and possible)
        size_t  bytesParsed = _requestParser.parse(_socket.getRecvStash()); 
        if (!_requestParser.isDone() && _socket.canRecv()) {
            _socket.fillStash();
            bytesParsed += _requestParser.parse(_socket.getRecvStash());
        }
        _socket.consumeRecvStash(bytesParsed);
        if (_requestParser.isDone()) {
            std::cerr << "REQUEST" << std::endl;
            _request.printMessage();
            std::cerr << std::endl << std::endl;
            std::cerr << "RESPONSE" << std::endl;
            _response.printMessage();
            std::cerr << std::endl << std::endl;
            _moveOnFromParsing();
        }
    }
    if (_stateMachine.getCurrentState() == RECV_BODY) {
        size_t  bytesConsumed = _requestPerformer.perform(_socket.getRecvStash());
        if (!_requestPerformer.isDone() && _socket.canRecv()) {
            _socket.fillStash();
            bytesConsumed = _requestPerformer.perform(_socket.getRecvStash());
        }
        _socket.consumeRecvStash(bytesConsumed);
        if (_requestPerformer.isDone()) {
            _moveOnFromParsing();
        }
    }
    if (_stateMachine.getCurrentState() == SEND_RESPONSE) {
        _socket.addToSendStash(_responseSender.getMessageToSend(BUFFER_SIZE - _socket.getSendStash().size())); //TODO Check if this number can never be negative!!! 
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