/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:27:30 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 11:34:30 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"

HttpResponse::status_code	HttpParser::httpSanitizer(std::string requestLine)
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
HttpResponse::status_code	HttpParser::parseRequestLine(HttpRequest& request, std::string requestLine)
{
	//Initial sanitizer
	HttpResponse::status_code	sanitizerResult = httpSanitizer(requestLine);
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
	std::size_t separatorPos = requestLine.find(' ');
    request.setTarget(requestLine.substr(0, separatorPos));
    requestLine.erase(0, separatorPos + 1);

	//Make sure the version is correct
	std::string	version = "HTTP/1.1\r\n";
	if (requestLine.compare(version) != 0)
        return (400);
	
	//All is good
	return (200);
}

HttpResponse::status_code	HttpParser::parseHeaderField(HttpRequest& request, std::string headerField)
{
	std::string	fieldName;
	std::string	fieldValue;
	
	//Parse fieldName	
	const size_t  colonPos = headerField.find(':');
    if (colonPos == headerField.npos)
        return (400);
    fieldName = headerField.substr(0, colonPos);
    if (fieldName.find_first_not_of(TOKEN_CHARS) != fieldName.npos)
        return (400);
	
	//Parse fieldValue
	fieldValue = headerField.substr(colonPos + 1);
    std::string::iterator   it = fieldValue.begin();
    while (*it == ' ' || *it == '\t')
        it++;
    fieldValue.erase(fieldValue.begin(), it);
    std::string::reverse_iterator   rit = fieldValue.rbegin();
    while (*rit == ' ' || *rit == '\t')
        rit++;
    fieldValue.resize(fieldValue.length() - (rit - fieldValue.rbegin()));
	//Additional checking can be done here:
	
	//Place the header in the request.
	try {
		request.getHeaders().at(fieldName); //TODO: pass this check to inside of the addHeader func
	} catch (std::out_of_range& e) {
		request.addHeader(fieldName, fieldValue);
	}
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