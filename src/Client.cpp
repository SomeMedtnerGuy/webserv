/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:39:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/09 13:16:32 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int id, struct pollfd& sockfd, ConfigFile& configFile)
    : _id(id), _socket(sockfd), _configFile(configFile),
        _activeRequest(NULL), _closeConnection(false), _timeoutTime(60)
{
    _lastActionTime = getCurrentTimestamp();
}
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
        if (_socket.wasActionMade()) {
            _lastActionTime = getCurrentTimestamp();
        } else if (hasTimedOut(_lastActionTime, _timeoutTime)) { //TODO: Must find way to update _lastActionTime. Perhaps a member of Socket which it can update every time recv or send is called?
            _setCloseConnection(true);
        }
    } while (_isNewRequestRequired() && !shouldCloseConnection());
}

int     Client::getId(void) const {return (_id);}

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