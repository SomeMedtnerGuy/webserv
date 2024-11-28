/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:24:14 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/11/27 18:21:22 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"

class Server {
private:
	std::string _port;
	std::string _serverName;
	std::string _host;
	std::string _root;
	std::string _clientLimit;
	std::string _index;
	std::string _errorPage;	

public:
	Server();
	Server(const Server& src);
	Server& operator=(const Server& src);
	~Server();

	/* Setters */
	void setPort(const std::string& port);
	void setServerName(const std::string& serverName);
	void setHost(const std::string& host);
	void setRoot(const std::string& root);
	void setClientLimit(const std::string& clientLimit);
	void setIndex(const std::string& index);
	void setErrorPage(const std::string& errorPage);

	/* Getters */
	std::string getPort(void) const;
	std::string getServerName(void) const;
	std::string getHost(void) const;
	std::string getRoot(void) const;
	std::string getClientLimit(void) const;
	std::string getIndex(void) const;
	std::string getErrorPage(void) const;

	void setElements(std::string element);
};

typedef void (Server::*SetFunct)();
size_t getElementNbr(std::string element);

#endif // SERVER_HPP