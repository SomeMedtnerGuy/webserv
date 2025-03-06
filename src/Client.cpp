/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:39:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 18:32:17 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Socket& socket, ConfigFile& configFile)
    : _socket(socket), _configFile(configFile), _activeRequest(NULL), _closeConnection(false){}
Client::~Client()
{
    if (_activeRequest) {
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
                _activeRequest = NULL;
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