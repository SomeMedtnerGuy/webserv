/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:29:05 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 15:55:45 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* PUBLIC */
Socket::Socket(int sockfd): _sockfd(sockfd){}
Socket::~Socket(){}

void	Socket::setCanRead(bool value) {_canRead = value;}
void	Socket::setCanWrite(bool value) {_canWrite = value;}
void	Socket::resetState(void) {setCanRead(false); setCanWrite(false);}

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

/* PRIVATE */
bool	Socket::_getCanRead(void) const {return (_canRead);}
bool	Socket::_getCanWrite(void) const {return (_canWrite);}