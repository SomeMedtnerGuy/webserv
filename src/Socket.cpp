/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:18:23 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/07 11:39:07 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* PUBLIC */
Socket::Socket(sockfd_t& sockfd)
    :_sockfd(sockfd), _canRecv(false), _canSend(false) {}
Socket::~Socket(){}

int    Socket::getSockFd(void) const
{
    return (_sockfd.fd);
}
void    Socket::updateFlags(void)
{
    if (_sockfd.revents & POLLIN) {
        _setCanRecv(true);
    }
    if (_sockfd.revents & POLLOUT) {
        _setCanSend(true);
    }
}
const Socket::data_container_t&   Socket::getRecvStash(void) const
{
    return (_recvStash);
}
void                Socket::consumeRecvStash(size_t byteAm)
{
    if (byteAm > _recvStash.size()) {
        _recvStash.clear();
    } else {
        _recvStash.erase(_recvStash.begin(), _recvStash.begin() + byteAm);
    }
}
const Socket::data_container_t&   Socket::getSendStash(void) const
{
    return (_sendStash);
}
void                Socket::addToSendStash(const data_container_t bytes)
{
    _sendStash.insert(_sendStash.end(), bytes.begin(), bytes.end());
}
void                Socket::clearStashes(void)
{
    _recvStash.clear();
    _sendStash.clear();
}
bool                Socket::canRecv(void) const
{
    return (_getCanRecv());
}
bool                Socket::canSend(void) const
{
    return (_getCanSend() && _sendStash.size() != 0);
}
void                Socket::fillStash(void)
{
    // That means this function was called when it shouldn't have been!
    if (!canRecv()) {
        throw (std::runtime_error("fill() was called when it should not have!"));
    } 
	std::memset(_buffer, 0x0, BUFFER_SIZE);
	ssize_t	recvOutput = recv(_sockfd.fd, _buffer, BUFFER_SIZE, 0); //Last arg are flags
	if (recvOutput <= 0) {
		throw (SocketException(SocketException::RECV,
            static_cast<SocketException::ActionReturn>(recvOutput)));
	}
	_recvStash.insert(_recvStash.end(), _buffer, _buffer + recvOutput);
	_setCanRecv(false);
}

void                Socket::flushStash(void)
{
    // That means this function was called when it shouldn't have been!
    if (!canSend()) {
        throw (std::runtime_error("flush() was called when it should not have!"));
    }
    std::memset(_buffer, 0x0, BUFFER_SIZE);
    //Make sure that you don't try to send more bytes than the buffer allows!
    size_t  bytesToSend = std::min(_sendStash.size(), static_cast<std::size_t>(BUFFER_SIZE));
    std::memcpy(_buffer, _sendStash.data(), bytesToSend);
    ssize_t bytesSent = send(_sockfd.fd, _buffer, bytesToSend, 0);
    if (bytesSent <= 0) {
        throw (SocketException(SocketException::SEND,
            static_cast<SocketException::ActionReturn>(bytesSent)));
    }
    _sendStash.erase(_sendStash.begin(), _sendStash.begin() + bytesSent);
    _setCanSend(false);
}

/* EXCEPTION */
Socket::SocketException::SocketException(Action action, ActionReturn actionReturn)
    :_action(action), _actionReturn(actionReturn)
{
    std::string actionStr = ((_action == RECV) ? "RECV" : "SEND");
    std::string actionReturnStr = ((_actionReturn == ERROR)
                                    ? "an error in socket occurred" 
                                    : "the client closed the connection");
    _message = actionStr + " action reported that " + actionReturnStr + "!"; 
}
Socket::SocketException::~SocketException() throw() {}
const char* Socket::SocketException::what() const throw()
{
    return (_message.c_str());
}

/* PRIVATE */
void    Socket::_setCanRecv(bool canRecv) {_canRecv = canRecv;}
bool    Socket::_getCanRecv(void) const {return (_canRecv);}
void    Socket::_setCanSend(bool canSend) {_canSend = canSend;}
bool    Socket::_getCanSend(void) const {return (_canSend);}