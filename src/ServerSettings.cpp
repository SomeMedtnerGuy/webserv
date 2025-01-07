/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:35:43 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/01/07 19:44:44 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerSettings.hpp"

ServerSettings::ServerSettings(ConfigFile& src) : _src(src)
{

	_serverName = "default";
	_root = "./root";
	_clientBodySize = 1000 * 1000;
	_index = "index.html";
	_errorPages[400] = "./.default/400.html";
	_errorPages[404] = "./.default/404.html";
	_errorPages[414] = "./.default/414.html";
	_errorPages[431] = "./.default/431.html";
	_errorPages[501] = "./.default/501.html";
	_allowMethods.push_back(strToMethod("GET"));
	_allowMethods.push_back(strToMethod("POST"));
	_allowMethods.push_back(strToMethod("DELETE"));
	_autoindex = false;
	_returnCode = -1;
}

ServerSettings::~ServerSettings(){}

/* Getters */

const std::string& ServerSettings::getServerName() const
{
	return (_serverName);
} 

const std::string& ServerSettings::getRoot() const
{
	return (_root);
}

size_t ServerSettings::getClientBodySize() const
{
	return (_clientBodySize);
}

const std::string& ServerSettings::getIndex() const
{
	return (_index);
}

const std::string& ServerSettings::getErrorPage(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
	if (it != _errorPages.end())
	{
		std::ifstream file(it->second.c_str());
		if (file.is_open())
			return it->second;
	}
	
	static const std::string defaultPage = ".default/" + intToStr(errorCode) + ".html"; /* check this, maybe creat a default for that code */
	return (defaultPage);
}

bool ServerSettings::getAllowMethod(Method method) const
{
	std::vector<Method>::const_iterator it = std::find(_allowMethods.begin(), _allowMethods.end(), method);
	
	if (it != _allowMethods.end())
		return (true);
	return (false);
}

bool ServerSettings::getAutoIndex() const
{
	return (_autoindex);
}

int ServerSettings::getReturnCode() const
{
	return (_returnCode);
}

const std::string& ServerSettings::getReturnURL() const
{
	return (_returnURL);
}

/* Setters */

void ServerSettings::setServer(std::string serverName)
{
	Server aux = _src.getServer(serverName);

	_serverName = aux.getServerName();
	_root = aux.getRoot();
	_clientBodySize = aux.getClientBodySize();
	setIndex(aux);
	_errorPages = aux.getErrorPage();
}

void ServerSettings::setIndex(Server& server)
{
	for (size_t i = 0; i < server.getIndexSize(); i++)
	{
		std::string filePath = server.getRoot() + "/" + server.getIndex(i);  
		std::ifstream file(filePath.c_str());
		if (file.is_open())
		{
			_index = server.getIndex(i);
			return;
		}
	}
}

void ServerSettings::setLocation(std::string target)
{
	std::vector<Location> auxVec = _src.getServer(_serverName).getLocation();
	
	for (std::vector<Location>::const_iterator it = auxVec.begin(); it != auxVec.end(); ++it)
	{
		if (!it->getSpecificPath().compare(target))
		{
			_autoindex = it->getAutoIndex();
			_root = it->getRoot();
			setAllowMethods(*it);
			setReturn(*it);
			setIndexLocation(*it);
			return ;
		}
	}	
}

void ServerSettings::setIndexLocation(Location location)
{
	for (size_t i = 0; i < location.getIndexSize(); i++)
	{
		std::string filePath = location.getRoot() + "/" + location.getIndex(i);  
		std::ifstream file(filePath.c_str());
		if (file.is_open())
		{
			_index = location.getIndex(i);
			return;
		}
	}
}

void ServerSettings::setAllowMethods(Location location)
{
	_allowMethods.clear();
	
	if (location.getAllowMethods("GET"))
		_allowMethods.push_back(strToMethod("GET"));
	else if (location.getAllowMethods("POST"))
		_allowMethods.push_back(strToMethod("POST"));
	else if (location.getAllowMethods("DELETE"))
		_allowMethods.push_back(strToMethod("DELETE"));
}

void ServerSettings::setReturn(Location location)
{
	std::vector<std::string> returnVec = location.getReturn();
	if (returnVec == std::vector<std::string>())
		return ;
	if (returnVec.size() == 2)
	{
		_returnCode = atoi(returnVec[0].c_str());
		_returnURL = returnVec[1];
	}
	else
		_returnURL = returnVec[0];
}