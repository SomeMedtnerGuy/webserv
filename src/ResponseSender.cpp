/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:13:08 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/28 12:12:22 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseSender.hpp"

ResponseSender::ResponseSender(HttpRequest& request, HttpResponse& response)
    : AMessageHandler(request, response), _file(), _headersSent(false)
{}
ResponseSender::~ResponseSender(){}

ResponseSender::data_t    ResponseSender::getMessageToSend(size_t byteLimit)
{
    if (!_file.is_open()) {
        _file.open(_response.getBodyPath().c_str(), std::ios::binary);;
    }
    Socket::data_container_t    output;
    if (!_headersSent) {
        std::string headerSection(_generateResponseHeader());
        output.insert(output.end(), headerSection.begin(), headerSection.end());
    } else {
        unsigned char   buffer[byteLimit];
        _file.read(reinterpret_cast<char*>(buffer), byteLimit - std::strlen(DELIMITOR));
        output.insert(output.end(), buffer, buffer + _file.gcount());
        if (_file.eof()) {
            output.insert(output.end(), '\r');
            output.insert(output.end(), '\n');
            _file.close();
            _setIsDone(true);
        }
    }
    
    //TODO
    /* THE FOLLOWING LINES ARE DEBUG
    _setIsDone(true);
    (void)byteLimit;
    Socket::data_container_t    output(0);
    output.insert(output.end(), 'l');
    output.insert(output.end(), 'o');
    output.insert(output.end(), 'l');
    std::cerr << "loles: " << output.size() << std::endl;*/
    return (output);
}

std::string	ResponseSender::_generateResponseHeader(void)
{
    std::stringstream response;    
    //Status line:
    response << "HTTP/1.1 " << _response.getStatusCode() << DELIMITOR;
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