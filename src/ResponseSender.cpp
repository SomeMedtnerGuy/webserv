/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:13:08 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/11 12:41:38 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseSender.hpp"

ResponseSender::ResponseSender(HttpRequest& request, HttpResponse& response)
    : AMessageHandler(request, response), _file(), _headersSent(false)
{}
ResponseSender::~ResponseSender(){}

ResponseSender::data_t    ResponseSender::getMessageToSend(size_t byteLimit)
{
    Socket::data_container_t    output;
    if (!_headersSent) {
        std::string headerSection(_generateResponseHeader());
        output.insert(output.end(), headerSection.begin(), headerSection.end());
    } else {
        if (_response.getStatusCode() == 204) {
            _setIsDone(true);
            return (output);
        }
        if (!_file.is_open()) {
            _file.open(_response.getBodyPath().c_str(), std::ios::binary);
        }
        unsigned char   buffer[byteLimit];
        _file.read(reinterpret_cast<char*>(buffer), byteLimit - std::strlen(DELIMITOR));
        output.insert(output.end(), buffer, buffer + _file.gcount());
        if (_file.eof()) {
            _file.close();
            _setIsDone(true);
        }
    }
    return (output);
}

std::string	ResponseSender::_generateResponseHeader(void)
{
    std::stringstream response;    
    //Status line:
    response << "HTTP/1.1 " << _response.getStatusCode() << DELIMITOR;
    std::cerr << "ststus code: " << _response.getStatusCode() << std::endl;
    //Headers:
    const HttpMessage::headers_dict headers = _response.getHeaders();
    HttpMessage::headers_dict::const_iterator cit;
    for (cit = headers.begin(); cit != headers.end(); cit++)
        response << cit->first << ": " << cit->second << DELIMITOR;
    response << DELIMITOR;
    std::string sendBuff(response.str());
    _headersSent = true;
    return (sendBuff);;
    
}