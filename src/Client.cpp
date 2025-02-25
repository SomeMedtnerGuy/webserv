/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:39:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/25 19:32:19 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* PUBLIC */
Client::Client(Socket& socket)
    : _socket(socket), _activeRequest(NULL), _closeConnection(false){}
Client::~Client()
{
    if (!_activeRequest) {
        delete _activeRequest;
    }
}

void    Client::handle(void)
{
    
    while (true) {
        _socket.updateFlags(); // Makes sure the flags are up to date with the revents set by poll().
        // Creates the conditions to remain in the loop
        bool    mustCreateNewRequest = !_activeRequest && _socket.getRecvStash().size();
        bool    mustReceive = _activeRequest 
                                && _activeRequest->isReceiving()
                                && _socket.canRecv();
        bool    mustSend = _activeRequest
                                && _activeRequest->isSending()
                                && _socket.canSend();
        
        if (mustCreateNewRequest) {
            _activeRequest = new RequestHandler();
        }
        else if (mustReceive) {
            _socket.fill();
            _activeRequest->handle();
        }
        else if (mustSend) {
            _activeRequest->handle();
            _socket.flush();
            //Must delete the request handler if it finished its job
            if (_activeRequest->isDone()) {
                delete _activeRequest;
                _activeRequest = NULL;
            }
        }
        else { // if none of the above was true, get out
            break ;
        }
    }
}

bool    Client::shouldCloseConnection(void)
{
    return (_closeConnection);
}

