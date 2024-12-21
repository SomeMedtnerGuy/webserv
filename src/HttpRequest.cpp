/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:50:00 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/21 21:13:27 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpRequest.hpp"

#include <iostream>
#include <cstring> // For memset
#include "../include/RequestParser.hpp"


//Constructors
HttpRequest::HttpRequest(int sockfd)
    : _parserDone(false), _sockfd(sockfd), _recvStatus(0){}
HttpRequest::~HttpRequest(){}

//Getters
bool    HttpRequest::eof(void) const {return (_eof);}
ssize_t HttpRequest::recvStatus(void) const {return (_recvStatus);}


int    HttpRequest::handleRequest(void)
{
    if (!_parserDone)
        _parseRequest();
    if (_parserDone)
        // Receive body if existent
        _eof = true; //TEMP
    return (_recvStatus);
}

void    HttpRequest::_parseRequest(void)
{
    std::string requestHeader;
    size_t      endOfHeader = requestHeader.npos;
    
    while (requestHeader.length() < MAX_REQUEST_HEADER_SIZE)
    {
        std::memset(_buffer, '\0', BUFFERSIZE);
        _recvStatus = recv(_sockfd, _buffer, BUFFERSIZE - 1, 0);
        if (_recvStatus <= 0)
            return ;
        requestHeader.append(_buffer);
        endOfHeader = requestHeader.find("\r\n\r\n");
        if (endOfHeader != requestHeader.npos)
        {
            _buffRemainder.assign(requestHeader.substr(endOfHeader + 4));
            requestHeader.erase(endOfHeader);
            break ;
        }
    }
    RequestParser   rp(requestHeader);
    rp.parse();
    _parserDone = true;
}

