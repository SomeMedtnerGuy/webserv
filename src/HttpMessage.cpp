/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 10:30:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/02 19:38:55 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMessage.hpp"

/* PUBLIC */
/* Canonical */
HttpMessage::HttpMessage(){}
HttpMessage::~HttpMessage() {}
/* Setters */
void	HttpMessage::addHeader(std::string name, std::string value)
{
    //TODO: Add logic in case name already exists
    _headers[name] = value;
}
/* Getters */
const HttpMessage::headers_dict&	HttpMessage::getHeaders(void) const {return (_headers);}

/* PROTECTED */
/* Canonical */
HttpMessage::HttpMessage(const HttpMessage& other){*this = other;}
HttpMessage&    HttpMessage::operator=(const HttpMessage& other)
{
    if (this != &other)
    {
        _headers = other.getHeaders();
    }
    return (*this);
}