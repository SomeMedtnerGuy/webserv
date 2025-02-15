/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:27:30 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 17:13:23 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"

HttpParser::code	HttpParser::httpSanitizer(std::string requestLine)
{
	size_t  crPos = requestLine.find('\r');;
    do
    {
        if (crPos != requestLine.npos && requestLine[crPos + 1] != '\n')
            return (400);
        crPos++;
        crPos = requestLine.find('\r', crPos);
    } while (crPos != requestLine.npos);
	return (200);
}
HttpParser::code	HttpParser::parseRequestLine(HttpRequest& request, std::string requestLine)
{
	//Initial sanitizer
	code	sanitizerResult = httpSanitizer(requestLine);
	if (sanitizerResult != 200)
		return (sanitizerResult);
	if (requestLine.compare(0, 2, "\r\n") == 0)
        requestLine.erase(0, 2);

	//Set Method
    request.setMethod(strToMethod(requestLine));
    requestLine.erase(0, requestLine.find(" ") + 1);
    if (request.getMethod() == HttpRequest::UNKNOWN)
        return (501);
	
	//Set target
	size_t separatorPos = requestLine.find(' ');
    request.setTarget(requestLine.substr(0, separatorPos));
    requestLine.erase(0, separatorPos + 1);

	//Make sure the version is correct
	std::string	version = "HTTP/1.1\r\n";
	if (requestLine.compare(version) != 0)
        return (400);
	
	//All is good
	return (200);
}



HttpRequest::Method  HttpParser::strToMethod(std::string str)
{
    const std::string methods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++)
	{
		if (str.compare(0, methods[i].length() + 1, methods[i] + " ") == 0)
			return (static_cast<HttpRequest::Method>(i));
	}
	return (HttpRequest::UNKNOWN);
}