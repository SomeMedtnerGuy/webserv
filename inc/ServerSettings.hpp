/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:43:52 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/11 18:11:31 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSETTINGS_HPP
#define SERVERSETTINGS_HPP

#include <iostream>
#include "ConfigFile.hpp"

class ServerSettings {
private:
	int _port;
	std::string _serverName;
	std::string	_location;
	std::string _root;
	size_t _clientBodySize;
	std::string _index; /* The first file found will be served */
	std::map<int, std::string> _errorPages;
	
	std::vector<Method> _allowMethods;
	bool _autoindex;
	int _returnCode;
	std::string _returnURL;
	
	ConfigFile& _src;

	void setIndex(Server& server);
	void setAllowMethods(Location location);
	void setReturn(Location location);
	void setIndexLocation(Location location);
	
public:
	ServerSettings(ConfigFile& src, int port);
	ServerSettings(const ServerSettings& src);
	ServerSettings& operator=(const ServerSettings& src);
	~ServerSettings();

	void setServer(std::string serverName);
	void setLocation(std::string target);

	const std::string& getServerName() const;
	const std::string getLocation() const;
	const std::string& getRoot() const;
	size_t getClientBodySize() const;
	const std::string& getIndex() const;
	const std::string getErrorPage(int errorCode) const;
	bool getAllowMethod(Method method) const;
	bool getAutoIndex() const;
	int getReturnCode() const;
	const std::string& getReturnURL() const;
};

#endif // SERVERSETTINGS_HPP
/*
SETTINGS:
-serverName: 			"default"
-root: 					"./root"
-clientBodySizeLimit:	1000*1000
-index:					"index.html"
-errorPages:				<error>: "./.default/<error>.html" (where error is the error code)
-allowMethods:			GET, POST, DELETE (the enum items)
-autoindex:				false
-returnCode:			-1
-returnURL:				""				
*/