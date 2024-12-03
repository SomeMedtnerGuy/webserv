/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:17:03 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/12/03 16:50:17 by nsouza-o         ###   ########.fr       */
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

void Server::setServerId(size_t serverId)
{
	_serverId = serverId;
}

void Server::setPort(const std::vector<std::string>& port)
{
	if (port.size() != 2)
		throw std::runtime_error("Port directive must have exactly one value, and can't be empty.");
	
	for (size_t i = 0; i < port[1].size(); ++i)
	{
		if (!isdigit(port[1][i]) && i != port[1].size() - 1)
			throw std::runtime_error("The value of Port directive must have only digits.");
		if (i == port[1].size() - 1 && port[1][i] != ';')
			throw std::runtime_error("The Port directive in server " + intToStr(_serverId) + " block does not end with \';\'");			
	}
	
	std::string portNb = port[1];
	portNb.erase(port[1].size() - 1);
	long nbr = atoi(portNb.c_str());
	if (nbr < 1024 || nbr > 65535)
		throw std::runtime_error("Invalid port number: The port must be in the range 1024-65535.");
	
	_port.push_back(nbr);
}

void Server::setServerName(const std::vector<std::string>& serverName)
{
	if (serverName.size() != 2)
		throw std::runtime_error("Server name directive must have exactly one value, and can't be empty.");
	
	for (size_t i = 0; i < serverName[1].size(); ++i)
	{
		if (i == serverName[1].size() - 1 && serverName[1][i] != ';')
			throw std::runtime_error("The Server name directive in server " + intToStr(_serverId) + " block does not end with \';\'");			
	}
	
	std::string serverNameNotConst = serverName[1];
	serverNameNotConst.erase(serverNameNotConst.size() - 1);
	if (serverNameNotConst.empty())
		return ;
	if (!validDomain(serverNameNotConst))/* TODO */
	{
		if (serverNameNotConst != "localhost" && serverNameNotConst != "LOCALHOST")
			throw std::runtime_error("Invalid server_name value.");
	}
	_serverName = serverNameNotConst;
}

void Server::setHost(const std::vector<std::string>& host)
{
	if (host.size() != 2)
		throw std::runtime_error("Host directive must have exactly one value, and can't be empty.");
	checkSemicolonAtEnd(host[1], _serverId, "Host");
	std::string lastHostElement = host[1];
	lastHostElement.erase(lastHostElement.size() - 1);	
	std::vector<std::string> splitedHost = splitStr(lastHostElement, '.');
	

	
	_host = lastHostElement;
}

void Server::setRoot(const std::vector<std::string>& root)
{
	_root = root;
}

void Server::setClientLimit(const std::vector<std::string>& clientLimit)
{
	_clientLimit = clientLimit;
}

void Server::setIndex(const std::vector<std::string>& index)
{
	_index = index;
}

void Server::setErrorPage(const std::vector<std::string>& errorPage)
{
	_errorPage = errorPage;
}

/* Getters */

int Server::getPort(size_t portNb) const
{
	if (portNb >= _port.size())
		throw std::runtime_error("Port number out of range. Valid range: 0 to " + intToStr(_port.size() - 1));
	return (_port[portNb]);
}

int Server::getPortSize(void) const
{
	return (_port.size());
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
	return (_root[0]);
}

std::string Server::getClientLimit(void) const
{
	return (_clientLimit[0]);
}

std::string Server::getIndex(void) const
{
	return (_index[0]);
}

std::string Server::getErrorPage(void) const
{
	return (_errorPage[0]);
}

/* -- */

void Server::setElements(std::string element)
{
	void (Server::*SetFunct[7])(const std::vector<std::string>&) = {
		&Server::setPort, 
		&Server::setServerName, 
		&Server::setHost, 
		&Server::setRoot, 
		&Server::setClientLimit, 
		&Server::setIndex, 
		&Server::setErrorPage
		};
	
	std::vector<std::string> elementVector = splitServerStr(element);
	size_t nbr =  getElementNbr(elementVector[0]);
	
	(this->*SetFunct[nbr])(elementVector);
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

