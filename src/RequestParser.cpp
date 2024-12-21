/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:05:55 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/21 21:17:37 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/RequestParser.hpp"

RequestParser::RequestParser(){}
RequestParser::RequestParser(std::string requestHeader)
:_requestHeader(requestHeader), _method(UNKNOWN), _statusCode(200)
{
    _methods.push_back("GET");
    _methods.push_back("POST");
    _methods.push_back("DELETE");
}
RequestParser::~RequestParser(){}

void    RequestParser::_preParser(void)
{
    size_t  crPos = _requestHeader.find('\r');;
    do
    {
        if (crPos != _requestHeader.npos && _requestHeader[crPos + 1] != '\n')
        {
            _statusCode = 400;
            break ;
        }
        crPos++;
        crPos = _requestHeader.find('\r', crPos);
    } while (crPos != _requestHeader.npos);
}

void    RequestParser::_parseMethod(void)
{
    if (_requestHeader.compare(0, 2, "\r\n") == 0)
        _requestHeader.erase(0, 2);
    std::vector<std::string>::iterator it;
    for (it = _methods.begin(); it != _methods.end(); it++)
    {
        if (_requestHeader.compare(0, (*it).length() + 1,*it + " ") == 0)
        {
            _method = static_cast<Method>(it - _methods.begin());
            _requestHeader.erase(0, (*it).length() + 1);
            break ;
        }
    }
    if (_method == UNKNOWN)
        _statusCode = 501;
}

void    RequestParser::_parseTarget(void)
{
    //TODO
}

void    RequestParser::_parseVersion(void)
{
    if (_requestHeader.compare(0, 8, "HTTP/1.1\r\n") != 0)
        _statusCode = 400;
    else
        _requestHeader.erase(0, 10);
}

void    RequestParser::_parseHeaders(void)
{
    //TODO
}

void    RequestParser::parse(void)
{
    /*TEST*/
    //std::cout << _requestHeader << std::endl;
    /*ENDTEST*/
    
    _preParser();
    _parseMethod();
    std::cout << _requestHeader << std::endl;
    _parseTarget();
    _parseVersion();
    _parseHeaders();
}