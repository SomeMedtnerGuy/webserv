/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:52:23 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 18:31:13 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

RequestManager::RequestManager(Socket& socket, ConfigFile& configFile)
    :_stateMachine(STATE_AM, RECV_HEADER), _socket(socket),
        _serverSettings(configFile, getPortFromSocket(_socket.getSockFd())),
        _requestParser(_request, _response, _serverSettings),
        _requestPerformer(_request, _response, _serverSettings),
        _responseSender(_request, _response),
        _request(), _response(_serverSettings),
        _handlingComplete(false), _closeConnection(false)
{
    _stateFunctionsMap[RECV_HEADER] = &RequestManager::_recvHeader;
    _stateFunctionsMap[RECV_BODY] = &RequestManager::_recvBody;
    _stateFunctionsMap[SEND_RESPONSE] = &RequestManager::_sendResponse;
}
RequestManager::~RequestManager(){}

void    RequestManager::handle(void)
{
    state_function  stateFunction;
    State           currentState = _stateMachine.getCurrentState();
    State           prevState = currentState;
    do {
        
        stateFunction = _stateFunctionsMap[_stateMachine.getCurrentState()];
        (this->*stateFunction)();
        prevState = currentState;
        currentState = _stateMachine.getCurrentState();
    } while (currentState != prevState);
}

bool    RequestManager::isDone(void) const {return (_getHandlingComplete());}
bool    RequestManager::shouldCloseConnection(void) const {return (_getCloseConnection());}


void    RequestManager::_setHandlingComplete(bool value) {_handlingComplete = value;}
bool    RequestManager::_getHandlingComplete(void) const {return (_handlingComplete);}
void    RequestManager::_setCloseConnection(bool value) {_closeConnection = value;}
bool    RequestManager::_getCloseConnection(void) const {return (_closeConnection);}

void    RequestManager::_recvHeader(void)
{
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
    _checkAndActOnErrors();
    if (_requestParser.isDone()) {
        _stateMachine.advanceState();
        _request.printMessage();
    }
}
void    RequestManager::_recvBody(void)
{
    size_t  bytesConsumed = _requestPerformer.perform(_socket.getRecvStash());
    if (!_requestPerformer.isDone() && _socket.canRecv()) {
        _socket.fillStash();
        bytesConsumed = _requestPerformer.perform(_socket.getRecvStash());
    }

    _socket.consumeRecvStash(bytesConsumed);
    _checkAndActOnErrors();
    if (_requestPerformer.isDone()) {
        _stateMachine.advanceState();
    }
}
void    RequestManager::_sendResponse(void)
{
    size_t  allowedSize = BUFFER_SIZE - std::min(_socket.getSendStash().size(),
                                                        static_cast<size_t>(BUFFER_SIZE));
    _socket.addToSendStash(_responseSender.getMessageToSend(allowedSize));
    if (_socket.canSend()) {
        _socket.flushStash();
    }
    if (_responseSender.isDone()) {
        _setHandlingComplete(true);
    }
}

void    RequestManager::_checkAndActOnErrors(void)
{
    ErrorSeverity   errorSeverity = _getErrorSeverity(_response.getStatusCode());
    switch (errorSeverity) {
        case NO_ERROR:
            break;
        case CONSUME_AND_ANSWER:
            _requestPerformer.activateConsumeMode();
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
        case 200: case 204:
            return (NO_ERROR);
        case 404: case 405: case 500: case 501: 
            return (CONSUME_AND_ANSWER);
        case 431: case 400:
            return (ANSWER_AND_CLOSE);
        case -1: // This is not a real code, is an internal indication that some bad shit happened
            return (CLOSE_IMMEDIATELY);
        default:
            throw (std::exception()); //TODO explicit
    }
}