/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:10:38 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/01 13:50:12 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMessage.hpp"

HttpMessage::HttpMessage():_headersSize(0){}
HttpMessage::~HttpMessage(){}

void	HttpMessage::addHeader(std::string fieldName, std::string fieldValue)
{
	try {
		_headers.at(fieldName);
	} catch (std::out_of_range& e) {
		_headers[fieldName] = fieldValue;
	}
}
const HttpMessage::headers_dict&	HttpMessage::getHeaders(void) const
{
	return (_headers);
}

void	HttpMessage::setHeadersSize(size_t newValue) {_headersSize = newValue;}
size_t	HttpMessage::getHeadersSize(void) const {return (_headersSize);}