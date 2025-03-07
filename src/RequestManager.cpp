/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:52:23 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/07 17:25:03 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

RequestManager::RequestManager(Socket& socket, ConfigFile& configFile)
    :_stateMachine(STATE_AM, RECV_HEADER), _socket(socket),
        _serverSettings(configFile, getPortFromSocket(_socket.getSockFd())),
        _requestParser(_request, _response, _serverSettings),
        _requestPerformer(_request, _response, _serverSettings),
        _responseSender(_request, _response),
        _cgiHandler(_request, _response, _serverSettings), 
        _handlingComplete(false), _closeConnection(false)
{}
RequestManager::~RequestManager(){}

void    RequestManager::handle(void)
{
    //TODO make the following logic into a switch
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

        _checkAndActOnErrors();
        if (_requestParser.isDone()) {
            _stateMachine.advanceState();
            _request.printMessage();
        }
    }
    
    if (_stateMachine.getCurrentState() == RECV_BODY) {
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
    
    if (_stateMachine.getCurrentState() == CGI_PROCESS)
    {        
        if (!CGIHandler::isCgi(_request.getTarget()))
	        _stateMachine.advanceState();
        else
        {
            if (!_cgiHandler.isCgiRunning())
                _cgiHandler.run();
            if (_cgiHandler.isCgiRunning() && _cgiHandler.cgiDone())
                _stateMachine.advanceState();
        }

    }
    
    if (_stateMachine.getCurrentState() == SEND_RESPONSE) {
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
}

bool    RequestManager::isDone(void) const {return (_getHandlingComplete());}
bool    RequestManager::shouldCloseConnection(void) const {return (_getCloseConnection());}


void    RequestManager::_setHandlingComplete(bool value) {_handlingComplete = value;}
bool    RequestManager::_getHandlingComplete(void) const {return (_handlingComplete);}
void    RequestManager::_setCloseConnection(bool value) {_closeConnection = value;}
bool    RequestManager::_getCloseConnection(void) const {return (_closeConnection);}

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
    std::cerr << statusCode << std::endl;
    switch (statusCode) {
        case 200: case 204:
            return (NO_ERROR);
        case 404: case 405: 
            return (CONSUME_AND_ANSWER);
        case 431:
            return (ANSWER_AND_CLOSE);
        case -1: // This is not a real code, is an internal indication that some bad shit happened
            return (CLOSE_IMMEDIATELY);
        default:
            throw (std::exception()); //TODO explicit
    }
}