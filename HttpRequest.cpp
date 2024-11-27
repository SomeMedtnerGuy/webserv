/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 21:02:34 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/11/27 22:08:49 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest(){}
HttpRequest::HttpRequest(char* request): _methods({"GET", "POST", "DELETE"})
{
	char			buff[7] = {};
	unsigned int	size = 0;

	while (request[size] != ' ' || size < 7)
		size++;
	memcpy(buff, request, size);
	std::string	buffStr = buff;
	for (int i=0; i<3; i++)
	{
		if (std::equal(buffStr.begin(), buffStr.end(), _methods[i]))
			_method = static_cast<Method>(i);
	}
}
HttpRequest::HttpRequest(const HttpRequest& other){}
HttpRequest&	HttpRequest::operator=(const HttpRequest& other)
{
	if (this != &other)
	{
		_method = other._method;
	}
	return (*this);
}
HttpRequest::~HttpRequest(){}


