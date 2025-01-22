/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:55:49 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/22 16:33:56 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

/* PUBLIC */
/* Canonical methods */
RequestParser::RequestParser(std::string requestHeader, HttpRequest& request)
:_requestHeader(requestHeader), _request(request){}
RequestParser::~RequestParser(){}

void    RequestParser::parse(void)
{
    _preParser();
    _parseMethod();
    _parseTarget();
    _parseVersion();
    _parseHeaders();
}

/* PRIVATE */
void    RequestParser::_preParser(void)
{
    size_t  crPos = _requestHeader.find('\r');;
    do
    {
        if (crPos != _requestHeader.npos && _requestHeader[crPos + 1] != '\n')
            throw RequestHandler::HttpError(400);
        crPos++;
        crPos = _requestHeader.find('\r', crPos);
    } while (crPos != _requestHeader.npos);
}

void    RequestParser::_parseMethod(void)
{
    if (_requestHeader.compare(0, 2, "\r\n") == 0)
        _requestHeader.erase(0, 2);
    _request.setMethod(strToMethod(_requestHeader));
    _requestHeader.erase(0, _requestHeader.find(" ") + 1);
    if (_request.getMethod() == UNKNOWN)
        throw  (RequestHandler::HttpError(501));
}

void    RequestParser::_parseTarget(void)
{
    size_t separatorPos = _requestHeader.find(' ');
    _request.setTarget(_requestHeader.substr(0, separatorPos));
    _requestHeader.erase(0, separatorPos + 1);
}

void    RequestParser::_parseVersion(void)
{
    if (_requestHeader.compare(0, sizeof(VERSION) - 1, VERSION) != 0)
        throw  (RequestHandler::HttpError(400));
    else
        _requestHeader.erase(0, sizeof(VERSION) - 1);
}

void    RequestParser::_parseHeaders(void)
{
    while (_requestHeader.compare(0, 2, "\r\n") != 0)
    {
        std::string                 fieldName;
        std::string    fieldValue;
        size_t      fieldEnd = _requestHeader.find("\r\n");
        std::string field = _requestHeader.substr(0, fieldEnd);
        fieldName = _parseFieldName(field);
        fieldValue = _parseFieldValue(field);
        try {
            _request.getHeaders().at(fieldName);
            //TODO: Check how to add headers to same field
        } catch (std::out_of_range& e) {
            _request.addHeader(fieldName, fieldValue);
        }
        _requestHeader.erase(0, fieldEnd + 2);
    }
}

std::string RequestParser::_parseFieldName(std::string& field)
{
    size_t  colonPos = field.find(':');
    if (colonPos == field.npos)
        throw  (RequestHandler::HttpError(400));
    std::string output = field.substr(0, colonPos);
    if (output.find_first_not_of(TOKEN_CHARS) != field.npos)
        throw  (RequestHandler::HttpError(400));
    return (output);
}

std::string RequestParser::_parseFieldValue(std::string& field)
{
    std::string output = field.substr(field.find_first_of(':') + 1);
    std::string::iterator   it = output.begin();
    while (*it == ' ' || *it == '\t')
        it++;
    output.erase(output.begin(), it);
    std::string::reverse_iterator   rit = output.rbegin();
    while (*rit == ' ' || *rit == '\t')
        rit++;
    output.resize(output.length() - (rit - output.rbegin()));
    return (output);
}

/* Canonical Methods */
RequestParser::RequestParser(const RequestParser& other)
    : _requestHeader(other._requestHeader), _request(other._request) {}