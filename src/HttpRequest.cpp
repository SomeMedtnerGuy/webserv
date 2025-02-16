/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:16:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 11:28:52 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest(): _method(UNKNOWN), _target(""){}
HttpRequest::~HttpRequest(){}

void	HttpRequest::setMethod(Method method){_method = method;}
HttpRequest::Method	HttpRequest::getMethod(void) const {return (_method);}
void	HttpRequest::setTarget(std::string target) {_target = target;}
std::string	HttpRequest::getTarget(void) const {return (_target);}

void	HttpRequest::printMessage(void)
{
	std::vector<std::string> methods = {"GET", "POST", "DELETE", "UNKNOWN"};
	std::cerr << "Method: " << methods[_method] << std::endl;
	
	std::cerr << "Target: " << _target << std::endl;
	
	std::cerr << "Headers:" << std::endl;
	for (headers_dict::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		std::cerr << it->first << ": " << it->second << std::endl;
	}
}
