/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:17:03 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/11/27 19:37:07 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(){}

Server::Server(const Server& src)
{
	*this = src;
}

Server& Server::operator=(const Server& src)
{
	if (this != &src)
	{
		_port = src._port;
		_serverName = src._serverName;
		_host = src._host;
		_root = src._root;
		_clientLimit = src._clientLimit;
		_index = src._index;
		_errorPage	= src._errorPage;
	}
	return (*this);
}

Server::~Server(){}

/* Setters */

void Server::setPort(const std::string& port)
{
	_port = port;
}

void Server::setServerName(const std::string& serverName)
{
	_serverName = serverName;
}

void Server::setHost(const std::string& host)
{
	_host = host;
}

void Server::setRoot(const std::string& root)
{
	_root = root;
}

void Server::setClientLimit(const std::string& clientLimit)
{
	_clientLimit = clientLimit;
}

void Server::setIndex(const std::string& index)
{
	_index = index;
}

void Server::setErrorPage(const std::string& errorPage)
{
	_errorPage = errorPage;
}

/* Getters */

std::string Server::getPort(void) const
{
	return (_port);
}

std::string Server::getServerName(void) const
{
	return (_serverName);
}

std::string Server::getHost(void) const
{
	return (_host);
}

std::string Server::getRoot(void) const
{
	return (_root);
}

std::string Server::getClientLimit(void) const
{
	return (_clientLimit);
}

std::string Server::getIndex(void) const
{
	return (_index);
}

std::string Server::getErrorPage(void) const
{
	return (_errorPage);
}

/* -- */

void Server::setElements(std::string element)
{
	void (Server::*SetFunct[7])(const std::string&) = {
		&Server::setPort, 
		&Server::setServerName, 
		&Server::setHost, 
		&Server::setRoot, 
		&Server::setClientLimit, 
		&Server::setIndex, 
		&Server::setErrorPage
		};
	
	std::vector<std::string> elementVector = splitServerStr(element);
	if (elementVector.size() != 2)
		throw std::runtime_error("Each directive must have exactly one value. Found: " + element);
	size_t nbr =  getElementNbr(elementVector[0]);
	
	(this->*SetFunct[nbr])(elementVector[1]);
}

size_t getElementNbr(std::string element)
{
	std::string elementsKeys[7] = {
		"port",
		"server_name", 
		"host", 
		"root", 
		"client_limit", 
		"index", 
		"error_page"
		};
	
	for (size_t i = 0; i < 7; ++i) {
		if (element == elementsKeys[i])
			return (i);		
	}
	throw std::runtime_error("Invalid element on Configuration File: " + element);
}

