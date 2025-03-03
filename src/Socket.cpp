/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:18:23 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/03 13:39:10 by ndo-vale         ###   ########.fr       */
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
bool                Socket::canRecv(void)
{
    return (_getCanRecv());
}
bool                Socket::canSend(void)
{
    return (_getCanSend() && _sendStash.size() != 0);
}
void                Socket::fillStash(void)
{
    // That means this function was called when it shouldn't have been!
    if (!canRecv()) {
        std::cerr << "fill() was called when it should not have!" << std::endl;
        throw (-1); //TODO: specify the error better, but if this happens it is a bug in code!
    } 
	std::memset(_buffer, 0x0, BUFFER_SIZE);
	ssize_t	recvOutput = recv(_sockfd.fd, _buffer, BUFFER_SIZE, 0); //Last arg are flags
	if (recvOutput <= 0) {
		std::cerr << "recv returned " << (recvOutput == 0 ? "0" : "-1") << std::endl;
		throw (std::exception()); //TODO: specify the error better so poller can catch it
	}
	_recvStash.insert(_recvStash.end(), _buffer, _buffer + recvOutput);
	_setCanRecv(false);
}


#include <stdio.h>
void                Socket::flushStash(void)
{
    // That means this function was called when it shouldn't have been!
    if (!canSend()) {
        std::cerr << "flush() was called when it should not have!" << std::endl;
        throw (-1); //TODO: specify the error better, but if this happens it is a bug in code!
    }
    std::memset(_buffer, 0x0, BUFFER_SIZE);
    //Make sure that you don't try to send more bytes than the buffer allows!
    size_t  bytesToSend = std::min(_sendStash.size(), static_cast<std::size_t>(BUFFER_SIZE));
    std::memcpy(_buffer, _sendStash.data(), bytesToSend);
    ssize_t bytesSent = send(_sockfd.fd, _buffer, bytesToSend, 0);
    if (bytesSent <= 0) {
        std::cerr << "send returned " << (bytesSent == 0 ? "0" : "-1") << std::endl;
        throw (std::exception()); //TODO: specify the error better so poller can catch it 
    }
    _sendStash.erase(_sendStash.begin(), _sendStash.begin() + bytesSent);
    _setCanSend(false);
}

/* PRIVATE */
void    Socket::_setCanRecv(bool canRecv) {_canRecv = canRecv;}
bool    Socket::_getCanRecv(void) const {return (_canRecv);}
void    Socket::_setCanSend(bool canSend) {_canSend = canSend;}
bool    Socket::_getCanSend(void) const {return (_canSend);}