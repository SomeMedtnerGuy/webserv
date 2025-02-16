/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:29:05 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 23:11:09 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* PUBLIC */
Socket::Socket(int sockfd): _sockfd(sockfd), _writePos(BUFFER_SIZE), _canRead(0), _canWrite(0)
{
	std::memset(_buffer, 0x0, BUFFER_SIZE);
}
Socket::~Socket(){}

void	Socket::setCanRead(bool value) {_canRead = value;}
void	Socket::setCanWrite(bool value) {_canWrite = value;}
void	Socket::resetState(void) {setCanRead(false); setCanWrite(false);}

int								Socket::getSockfd(void) const {return (_sockfd);}
const Socket::data_container	Socket::getStash(void) const {return (_stash);}
void							Socket::consumeFromStash(size_t bytesAmount)
{
	if (bytesAmount > _stash.size())
		_stash.clear();
	else
		_stash.erase(_stash.begin(), _stash.begin() + bytesAmount);
}

ssize_t	Socket::readToStash(void)
{
	if (!_canRead) {return (-1);}
	//TODO: If a recv takes two requests at once, pollin wont accuse the second one after the first one is read
	//TODO: then probably it is better to use peek and only take what is necessary!!
	std::memset(_buffer, 0x0, BUFFER_SIZE);
	ssize_t	recvOutput = recv(_sockfd, _buffer, BUFFER_SIZE, 0); //Last arg are flags
	if (recvOutput <= 0)
	{
		std::cerr << "recv returned -1 or 0!" << std::endl;
		throw (std::exception()); //TODO: specify the error better so poller can catch it
	}
	_stash.insert(_stash.end(), _buffer, _buffer + recvOutput);
	setCanRead(false);
	return (recvOutput);
}

# include <stdio.h>
ssize_t	Socket::sendToSocket(const unsigned char* data, size_t byteAmount)
{
	if (!_canWrite) {return (-1);}
	if (_writePos == BUFFER_SIZE)
	{
		_writePos = 0;
		std::memset(_buffer, 0x0, BUFFER_SIZE);
		std::memcpy(_buffer, data, byteAmount);
	}
	ssize_t bytesSent = send(_sockfd, _buffer + _writePos, byteAmount - _writePos, 0);
	if (bytesSent <= 0)
	{
		std::cerr << "send returned -1 or 0!" << std::endl;
		throw (std::exception()); //TODO: specify the error better so poller can catch it
	}
	if (byteAmount == static_cast<size_t>(bytesSent))
		_writePos = BUFFER_SIZE;
	else
		_writePos += bytesSent;
	setCanWrite(false);
	return (_writePos); //TODO PROBABLY WRONG
}

void	Socket::printStash(void)
{
	data_container::iterator it;
	for (it = _stash.begin(); it != _stash.end(); it++)
	{
		std::cerr << *it;
	}
	std::cerr << std::endl;
}

/* PRIVATE */
bool	Socket::_getCanRead(void) const {return (_canRead);}
bool	Socket::_getCanWrite(void) const {return (_canWrite);}