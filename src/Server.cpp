/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:17:03 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/01/03 19:41:50 by nsouza-o         ###   ########.fr       */
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
		_listen = src._listen;
		_serverName = src._serverName;
		_root = src._root;
		_clientBodySize = src._clientBodySize;
		_index = src._index;
		_errorPage	= src._errorPage;
		_locations = src._locations;
	}
	return (*this);
}

Server::~Server(){}

/* Setters */

void Server::setServerId(size_t serverId)
{
	_serverId = serverId;
}

void Server::setListen(const std::vector<std::string>& port)
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
	
	_listen.push_back(nbr);
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
	if (!validDomain(serverNameNotConst))
	{
		if (serverNameNotConst != "localhost" && serverNameNotConst != "LOCALHOST")
			throw std::runtime_error("Invalid server_name value.");
	}
	_serverName = serverNameNotConst;
}

void Server::setRoot(const std::vector<std::string>& root)
{
	if (root.size() != 2)
		throw std::runtime_error("Root directive must not have more than one value");
	if (root[1].find("../") != std::string::npos)
		throw std::runtime_error("Root directive must not have '../'. Access to parent directories is not allowed.");
		
	checkSemicolonAtEnd(root[1], _serverId, "Root");
	std::string lastRootElement = root[1];
	lastRootElement.erase(lastRootElement.size() - 1);
	
	if (!isDirectory(lastRootElement))
			throw std::runtime_error("Root directive must have a directory path.");
	
	_root = lastRootElement;
}

void Server::setClientBodySize(const std::vector<std::string>& clientLimit)
{
	if (clientLimit.size() > 2)
		throw std::runtime_error("Host directive must not have more than one value");
	checkSemicolonAtEnd(clientLimit[1], _serverId, "Client Body Size");
	std::string lastClientLimit = clientLimit[1];
	lastClientLimit.erase(lastClientLimit.size() - 1);
	for (size_t j = 0; j < lastClientLimit.size(); j++)
	{
		if (!isdigit(lastClientLimit[j]))
			throw std::runtime_error("Invalid Client Limit directive: Expected a number value grater than 0.");
	}
	_clientBodySize = atoi(lastClientLimit.c_str());
}

void Server::setIndex(const std::vector<std::string>& index)
{
	
	if (index.size() < 2)
		throw std::runtime_error("Host directive must have one value, at least.");
	
	checkSemicolonAtEnd(index[index.size() - 1], _serverId, "Index");
	
	for (size_t i = 1; i < index.size(); i++)
	{
		if (i == index.size() - 1)
		{
			std::string lastIndex = index[i];
			lastIndex.erase(lastIndex.size() - 1);
			_index.push_back(lastIndex);
		}
		else
			_index.push_back(index[i]);
	}
}

void Server::setErrorPage(const std::vector<std::string>& errorPage)
{
	if (errorPage.size() != 3)
		throw std::runtime_error("The Error Page directive must have a error number and one path for a file.");
	
	checkSemicolonAtEnd(errorPage[2], _serverId, "Error Page");
	std::string value = errorPage[2];
	value.erase(errorPage[2].size() - 1);
	
	for (size_t j = 0; j < errorPage[1].size(); j++)
	{
		if (!isdigit(errorPage[1][j]))
			throw std::runtime_error("Invalid Error Page directive: Expected a number value.");
	}
	int errorNbr = atoi(errorPage[1].c_str());
	if (errorNbr < 100 || errorNbr >= 600)
		throw std::runtime_error("Invalid Error Page directive.");
	
	_errorPage[errorNbr] = value;
}

void Server::setLocation(std::vector<std::string>& serverVector, size_t i)
{
	checkCurlyBrace(serverVector[i], true);
	std::string specificPath = cacthPath(serverVector[i]);
	for (size_t j = i; j < serverVector.size() - 1; j++)
	{
		if (serverVector[j].find("}") != std::string::npos)
		{
			checkCurlyBrace(serverVector[j], false);
			_locations.push_back(fillLocation(serverVector, i + 1, j - 1));
		}
	}
	std::string location = "/";
}

Location Server::fillLocation(std::vector<std::string>& serverVector, size_t begin, size_t end)
{
	Location realLocation;
	std::string specificPath = cacthPath(serverVector[begin - 1]);
	realLocation.setSpecificPath(specificPath);
	for (size_t i = begin; i <= end; i++)
		realLocation.setLocationElements(serverVector[i]);
	
	return (realLocation);
}

/* Getters */

int Server::getListen(size_t portNb) const
{
	if (portNb >= _listen.size())
		throw std::runtime_error("Port number out of range. Valid range: 0 to " + intToStr(_listen.size() - 1));
	return (_listen[portNb]);
}

int Server::getListenSize(void) const
{
	return (_listen.size());
}

std::string Server::getServerName(void) const
{
	return (_serverName);
}

std::string Server::getRoot(void) const
{
	return (_root);
}

size_t Server::getClientBodySize(void) const
{
	return (_clientBodySize);
}

size_t Server::getIndexSize() const
{
	return (_index.size());
}

std::string Server::getIndex(size_t indexNbr) const
{
	if (indexNbr > _index.size() -1)
		throw std::runtime_error("Index number out of range. Valid range: 0 to " + intToStr(_index.size() -1));

	return (_index[indexNbr]);
}

const std::vector<Location>& Server::getLocation() const
{
	return(this->_locations);
}

const std::map<int, std::string>& Server::getErrorPage() const
{
	return (_errorPage);
}

/* -- */

void Server::setElements(std::string element)
{
	void (Server::*SetFunct[6])(const std::vector<std::string>&) = {
		&Server::setListen, 
		&Server::setServerName,
		&Server::setRoot, 
		&Server::setClientBodySize, 
		&Server::setIndex, 
		&Server::setErrorPage
		};
	
	std::vector<std::string> elementVector = splitServerStr(element);
	size_t nbr =  getElementNbr(elementVector[0]);
	
	(this->*SetFunct[nbr])(elementVector);
}

size_t getElementNbr(std::string element)
{
	std::string elementsKeys[6] = {
		"listen",
		"server_name", 
		"root", 
		"client_body_size", 
		"index", 
		"error_page"
		};
	
	for (size_t i = 0; i < 7; ++i) {
		if (element == elementsKeys[i])
			return (i);		
	}
	throw std::runtime_error("Invalid element on Configuration File: " + element);
}
