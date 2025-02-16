/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:30:00 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 11:38:44 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse(): _statusCode(200), _bodyPath("") {}
HttpResponse::~HttpResponse() {}

void						HttpResponse::setStatusCode(status_code statusCode) {_statusCode = statusCode;}
HttpResponse::status_code	HttpResponse::getStatusCode(void) const {return (_statusCode);}
void						HttpResponse::setBodyPath(std::string bodyPath) {_bodyPath = bodyPath;}
std::string					HttpResponse::getBodyPath(void) const {return (_bodyPath);}

void	HttpResponse::printMessage(void)
{
	std::cerr << "Status code: " << _statusCode << std::endl;
	
	std::cerr << "Body path: " << _bodyPath << std::endl;
	
	std::cerr << "Headers:" << std::endl;
	for (headers_dict::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		std::cerr << it->first << ": " << it->second << std::endl;
	}
}