/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:32:28 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/25 14:34:25 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

/* PUIBLIC */
/* Canonical */
HttpRequest::HttpRequest(): _method(UNKNOWN), _hasBody(false){}
HttpRequest::~HttpRequest(){}
/* Setters */
void    	HttpRequest::setMethod(Method method) {_method = method;}
void    	HttpRequest::setTarget(std::string target) {_target = target;}
void		HttpRequest::setHasBody(bool hasBody) {_hasBody = hasBody;}
/* Getters */
Method      HttpRequest::getMethod(void) const {return (_method);}
std::string HttpRequest::getTarget(void) const {return (_target);}
bool		HttpRequest::getHasBody(void) const {return (_hasBody);}

/* PRIVATE */
/* Canonical */
HttpRequest::HttpRequest(const HttpRequest& other)
    : HttpMessage(other), _method(other.getMethod()), _target(other.getTarget()) {}
HttpRequest&    HttpRequest::operator=(const HttpRequest& other)
{
    if (this != &other)
    {
        HttpMessage::operator=(other);
        _method = other.getMethod();
        _target = other.getTarget();
    }
    return (*this);
}