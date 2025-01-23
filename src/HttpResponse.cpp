/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:05:10 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/23 19:03:50 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

/* PUBLIC */
/* Canonical */
HttpResponse::HttpResponse(): _statusCode(200), _bodyPath(DEFAULT_PAGE_PATH)
{
    _headers["Content-Type"] = "text/html; charset=UTF-8";
    _headers["Server"] = "localhost";
    _headers["Content-Length"] = Helpers::ntostr(Helpers::getFileLength(_bodyPath));
}
HttpResponse::~HttpResponse(){}

void    HttpResponse::setStatusCode(int statusCode)
{
    _statusCode = statusCode;
    //TODO: In order for this to work, response must hold a reference to ServerSettings or send the path as argument (maybe better)
    // Later it will fetch the right path from there. Must do verification to check if it exists HERE
    setBodyPath("./.default/" + Helpers::ntostr(_statusCode) + ".html");
}
void    HttpResponse::setBodyPath(std::string bodyPath)
{
    _bodyPath = bodyPath;
    _headers["Content-Length"] = Helpers::ntostr(Helpers::getFileLength(_bodyPath));
	if (bodyPath.find(".jpg") != bodyPath.npos)
		_headers["Content-Type"] = "image/jpeg";
	else if (bodyPath.find(".pdf") != bodyPath.npos)
		_headers["Content-Type"] = "application/pdf";
}

int     HttpResponse::getStatusCode() const {return (_statusCode);}
std::string HttpResponse::getBodyPath() const {return (_bodyPath);}

/* PRIVATE */
/* Canonical */
HttpResponse::HttpResponse(const HttpResponse& other)
    : HttpMessage(other), _statusCode(other.getStatusCode()), _bodyPath(other.getBodyPath()) {}
HttpResponse&    HttpResponse::operator=(const HttpResponse& other)
{
    if (this != &other)
    {
        HttpMessage::operator=(other);
        _statusCode = other.getStatusCode();
        _bodyPath = other.getBodyPath();
    }
    return (*this);
}