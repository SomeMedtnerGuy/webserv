/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:24:14 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/12/19 11:03:25 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
#include "Location.hpp"

class Server {
private:
	int _serverId;
	std::vector<int> _port;
	std::string _serverName;
	std::string _host;
	std::string _root;
	size_t _clientLimit;
	std::vector<std::string> _index;
	std::map<std::string, std::string> _errorPage;	
	std::vector<Location> _locations;

public:
	Server();
	Server(const Server& src);
	Server& operator=(const Server& src);
	~Server();

	/* Setters */
	void setServerId(size_t serverId);
	void setPort(const std::vector<std::string>& port);
	void setServerName(const std::vector<std::string>& serverName);
	void setHost(const std::vector<std::string>& host);
	void setRoot(const std::vector<std::string>& root);
	void setClientLimit(const std::vector<std::string>& clientLimit);
	void setIndex(const std::vector<std::string>& index);
	void setErrorPage(const std::vector<std::string>& errorPage);
	void setLocation(std::vector<std::string>& serverVector, size_t i);
	Location fillLocation(std::vector<std::string>& serverVector, size_t begin, size_t end);

	/* Getters */
	int getPort(size_t portNb) const;
	int getPortSize(void) const;
	std::string getServerName(void) const;
	std::string getHost(void) const;
	std::string getRoot(void) const;
	size_t getClientLimit(void) const;
	size_t getIndexSize() const;
	std::string getIndex(size_t indexNbr) const;
	bool isErrorPageDefined(std::string key) const;
	std::string getErrorPage(std::string key) const;
	const Location* getLocation(std::string& path) const;

	void setElements(std::string element);
};

typedef void (Server::*SetFunct)();
size_t getElementNbr(std::string element);

#endif // SERVER_HPP