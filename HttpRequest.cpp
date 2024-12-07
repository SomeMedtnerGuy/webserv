/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 21:02:34 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/07 19:27:01 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest(){}
HttpRequest::HttpRequest(int sockfd):
	_currentState(REQUEST_LINE_PARSING), _sockfd(sockfd), _eof(false)
	//_rlp(), _hp(), _bp() 
{usleep(10000);} // NEEDED IF SOCKET IS NONBLOCKING
HttpRequest::HttpRequest(const HttpRequest& other){}
HttpRequest&	HttpRequest::operator=(const HttpRequest& other)
{
	if (this != &other)
	{
		_sockfd = other._sockfd;
	}
	return (*this);
}
HttpRequest::~HttpRequest(){}

void	HttpRequest::parseMoreRequest(void)
{
	std::memset(_buffer, '\0', BUFFER_SIZE);
	if (recv(_sockfd, _buffer, BUFFER_SIZE - 1, 0) <= 0) // SOCKFD MUST BE NON BLOCKING!! CHECK THIS PLZ
	{
		_eof = true;
		return ;
	}
	std::string	buff(_buffer);
	if (!_isBuffValid());
		throw HttpRequestException("Bare CR found! Invalid request.");
	_currentRequest += buff;
	switch (_currentState)
	{
		case REQUEST_LINE_PARSING:
			// TODO: continue request line parsing
		case HEADERS_PARSING:
			// TODO: continue headers parsing
		case BODY_PARSING:
			// TODO: continue body parsing
		default:
			std::cout << "LOLADA MIX" << std::endl;
	}
}

bool	HttpRequest::_isBuffValid(void)
{
	size_t	rPos = _currentRequest.find('\r');
	if (rPos != _currentRequest.npos 					// \r is found...
			&& rPos != _currentRequest.length() - 1		// and is not the last char...
			&& _currentRequest[rPos+1] != '\n')			// and has no \n after it
		return (false);
	return (true);
}



bool	HttpRequest::eof(void){return _eof;}

/*void	HttpRequest::parseRequest(void)
{
	_initialCheck();
	_parseMethod();
	_parseTarget();
	//_parseVersion();
	//_parseHeaders();
	//_parseBody();	recv(_sockfd, _buffer, BUFFER_SIZE, 0);
	
	while (_request[i])
	{
		if (_request[i] == '\r' && _request[i+1] != '\n')
			throw HttpRequestException("Bare CR found! Invalid request.");
		i++;
	}
}

void	HttpRequest::_parseMethod(void)
{
	char			buff[7] = {};
	unsigned int	size = 0;

	while (_request[size] != ' ' && size < MAX_METHOD_SIZE + 1)
		size++;
	memcpy(buff, _request, size);
	std::string	buffStr = buff;
	for (int i=0; i<METHODS_AM; i++)
	{
		if (buffStr == _methods[i])
			_method = static_cast<Method>(i);
	}
	if (_method == INVALID)
		throw HttpRequestException("Invalid Method");
	_requestPtr += size + 1; //should test this first
}

void	HttpRequest::_parseTarget(void)
{
	size_t	size = 0;

	while (!std::isspace(_requestPtr[size]) && size < MAX_REQUEST_SIZE)
		size++;
	if (size >= MAX_REQUEST_SIZE)
		throw HttpRequestException("Target too long");
	
	std::string target(_requestPtr, size);
	
	
	std::cout << target << std::endl;
}*/

HttpRequest::HttpRequestException::HttpRequestException(std::string msg): _message(msg) {}
HttpRequest::HttpRequestException::~HttpRequestException() throw() {}
const char*	HttpRequest::HttpRequestException::what() const throw() {return (_message.c_str());}