/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 21:02:34 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/11/28 21:18:08 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest(){}
HttpRequest::HttpRequest(char* request): _methods{"GET", "POST", "DELETE"},
	_request(request), _requestPtr(request), _method(INVALID)
{
	if (!_request)
		throw (HttpRequestException("Null request"));
}
HttpRequest::HttpRequest(const HttpRequest& other){}
HttpRequest&	HttpRequest::operator=(const HttpRequest& other)
{
	if (this != &other)
	{
		for (int i=0; i<METHODS_SIZE; i++)
		{
			_request = other._request;
			_method = other._method;
		}
	}
	return (*this);
}
HttpRequest::~HttpRequest(){}

void	HttpRequest::parseRequest(void)
{
	_parseMethod();
	_parseTarget();
	//_parseVersion();
	//_parseHeaders();
	//_parseBody();
}

void	HttpRequest::_parseMethod(void)
{
	char			buff[7] = {};
	unsigned int	size = 0;

	while (_request[size] != ' ' && size < 7)
		size++;
	memcpy(buff, _request, size);
	std::string	buffStr = buff;
	for (int i=0; i<METHODS_SIZE; i++)
	{
		if (buffStr == _methods[i])
			_method = static_cast<Method>(i);
	}
	if (_method == INVALID)
		throw HttpRequestException("Invalid Method");
	_requestPtr += size; //should test this first
}

void	HttpRequest::_parseTarget(void)
{
	int	size = 0;
	
	while (std::isspace(_requestPtr[size]) && size < 8000)
		size++;
}

HttpRequest::HttpRequestException::HttpRequestException(std::string msg): _message(msg) {}
HttpRequest::HttpRequestException::~HttpRequestException() throw() {}
const char*	HttpRequest::HttpRequestException::what() const throw() {return (_message.c_str());}