/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:39:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 19:43:47 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Socket& socket, ConfigFile& configFile)
    : _socket(socket), _configFile(configFile), _activeRequest(NULL), _closeConnection(false){}
Client::~Client()
{
    if (!_activeRequest) {
        delete _activeRequest;
    }
}

void    Client::handle(void)
{
    // Update flags only once per call
    _socket.updateFlags();

    do {
        if (_isNewRequestRequired()) {
            _activeRequest = new RequestManager(_socket, _configFile);
        }
        if (_activeRequest) {
            _activeRequest->handle(); //The bulk of the work is done here
            if (_activeRequest->isDone()) {
                _setCloseConnection(_activeRequest->shouldCloseConnection());
                delete _activeRequest;
            }
        }
    } while (_isNewRequestRequired() && !shouldCloseConnection());
}

bool    Client::shouldCloseConnection(void) const
{
    return (_getCloseConnection());
}

void    Client::_setCloseConnection(bool closeConnection) {_closeConnection = closeConnection;}
bool    Client::_getCloseConnection(void) const {return (_closeConnection);}

bool    Client::_isNewRequestRequired(void) const
{
    bool    outstandingDataExists = _socket.getRecvStash().size() > 0 
                                || _socket.canRecv();
    return (!_activeRequest && outstandingDataExists);
}


/*void    Client::handle(void)
{
    _socket.updateFlags();// Makes sure the flags are up to date with the revents set by poll().    
    while (true) {
        _socket.updateFlags(); 
        // Creates the conditions to remain in the loop
        
        bool    mustCreateNewRequest = !_activeRequest
                                && (_socket.getRecvStash().size()
                                    || _socket.canRecv());
        bool    mustReceive = _activeRequest 
                                && _activeRequest->isReceiving()
                                && _socket.canRecv();
        bool    mustSend = _activeRequest
                                && _activeRequest->isSending()
                                && _socket.canSend();
        bool    mustCleanup = _activeRequest && _activeRequest->isDone();
        
        if (mustCreateNewRequest) {
            std::cerr << "must create new request" << std::endl;
            _activeRequest = new RequestManager(_socket, _configFile);
        }
        else if (mustReceive) {
            std::cerr << "must receive" << std::endl;
            _socket.fillStash();
            _activeRequest->handle();
        }
        else if (mustSend) {
            std::cerr << "must send" << std::endl;
            _activeRequest->handle();
            _socket.flushStash();
        }
        else if (mustCleanup) {
            std::cerr << "must cleanup" << std::endl;
            _setCloseConnection(_activeRequest->shouldCloseConnection());
            delete _activeRequest;
            _activeRequest = NULL;
            if (shouldCloseConnection())
                break;
        } else {
            // if none of the above was true (for example, the request needs to read but
            //reading buffer is unavailable), get out and wait for the next iteration of the poller loop
            break ;
        }
    }
}

bool    Client::shouldCloseConnection(void)
{
    return (_getCloseConnection());
}


void    Client::_setCloseConnection(bool closeConnection) {_closeConnection = closeConnection;}
bool    Client::_getCloseConnection(void) const {return (_closeConnection);}*/

