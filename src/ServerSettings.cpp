/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:35:43 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/01/07 15:35:01 by nsouza-o         ###   ########.fr       */
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
		return it->second;
	
	static const std::string emptyString; /* check this */
	return emptyString;
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

