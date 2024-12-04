/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:24:14 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/12/04 17:23:13 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"

class Server {
private:
	int _serverId;
	std::vector<int> _port;
	std::string _serverName;
	std::string _host;
	std::string _root;
	size_t _clientLimit;
	std::vector<std::string> _index;
	std::vector<std::string> _errorPage;	

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

	/* Getters */
	int getPort(size_t portNb) const;
	int getPortSize(void) const;
	std::string getServerName(void) const;
	std::string getHost(void) const;
	std::string getRoot(void) const;
	size_t getClientLimit(void) const;
	std::string getIndex(void) const;
	std::string getErrorPage(void) const;

	void setElements(std::string element);
};

typedef void (Server::*SetFunct)();
size_t getElementNbr(std::string element);

#endif // SERVER_HPP