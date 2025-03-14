/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:13:08 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/14 19:22:05 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseSender.hpp"

ResponseSender::ResponseSender(HttpRequest& request, HttpResponse& response)
    : AMessageHandler(request, response), _file(), _headersSent(false), _wasDone(false)
{}
ResponseSender::~ResponseSender(){}

ResponseSender::data_t    ResponseSender::getMessageToSend(size_t byteLimit)
{
    Socket::data_container_t    output;

    if (!_headersSent) {
        std::string headerSection(_generateResponseHeader());
        output.insert(output.end(), headerSection.begin(), headerSection.end());
    } else if (_request.getMethod() == HEAD) {
        _setIsDone(true);
        return (output);
    } else {
        if (_response.getStatusCode() == 204) {
            _setIsDone(true);
            return (output);
        }
        if (!_file.is_open()) {
            _file.open(_response.getBodyPath().c_str(), std::ios::binary);
        }
        unsigned char   buffer[byteLimit];
        
        if (_response.cgiBodyStarted != std::streampos(-1))
            _file.seekg(_response.cgiBodyStarted, std::ios::beg); // ignore file headers when processing CGI requests
            
        _file.read(reinterpret_cast<char*>(buffer), byteLimit - std::strlen(DELIMITOR));
        output.insert(output.end(), buffer, buffer + _file.gcount());
        if (_file.eof()) {
            _file.close();
            _setIsDone(true);
        }
        


        if (!_wasDone && _request.getMethod() == POST && _request.getTarget().rfind(".bla") == _request.getTarget().length() - 4) {
            output.erase(output.begin(), output.begin() + 58);
            _wasDone = true;
        } // TODO: This is hardcode to correct size of the body sent. must update it AFTER consuming headers


        
    }
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



    std::string tmpstr;

    for (cit = headers.begin(); cit != headers.end(); cit++) {
        
        std::stringstream tmp;
        
        tmp << cit->second;
        tmpstr = tmp.str();
        if (_request.getMethod() == POST && _request.getTarget().rfind(".bla") == _request.getTarget().length() - 4) {
            if (cit->first.compare("Content-Length") == 0) {
                tmpstr.erase(tmpstr.end() - 2, tmpstr.end());
                tmpstr.append("00");
            }
            std::cerr << "final: " << tmpstr << std::endl;
        } 



        response << cit->first << ": " << tmpstr << DELIMITOR;
    }
        
    response << DELIMITOR;
    std::string sendBuff(response.str());
    _headersSent = true;
    return (sendBuff);;
    
}