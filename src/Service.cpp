/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 22:41:07 by joamonte          #+#    #+#             */
/*   Updated: 2024/12/09 10:19:27 by joamonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Service.hpp"
//#include "Parser.hpp"
#include "Utils.hpp"

bool	g_shutdown = false;

//Constructor and destructor

Service::Service(int ac, char **av)
{
	printMsg(START_MSG, GREEN);

	std::signal(SIGPIPE, SIG_IGN);
	std::signal(SIGINT, signalHandler);

	Parser	input(ac, av); //Criar função no parser
	this->_servers = input.getServerConfigs();
	this->_defaultServers = this->_countDefaultServers();
}

Service::~Service()
{
	for (size_t i = 0; i < this->_pollingRequests.size(); i++)
		close(this->_pollingRequests.at(i).fd);
	printMsg(END_MSG, GREEN);
}

// ---> Public member functions ----------------------------------------------

void Service::setup()
{
	printMsg(SETUP_MSG, YELLOW);

	this->_initAddressParameters();

	std::vector<std::string>::iterator server = this->_servers.begin();
	for(; server != this->_servers.end(); server++)
	{
		if (!server->getIsDefault())
			continue;

		this->_getSetupInfo(server);
		this->_setReuseableAddress();
		this->_convertHostToAddress();
		this->_bindAddressToSocket();
		this->_setSocketListening();
		this->_addSocketInPollingRequests();

		printMsg(SET_SERVER_MSG(this->_tmp.host, this->_tmp.port), BLUE);

		this->_resetInfo();
	}
}

void Service::launch()
{
	printMsg(LAUNCH_MSG, BLUE);

	while (g_shutdown == false)
	{
		this->_initPollingRequests();
		this->_pollingManager();
	}
}

void Service::printServersInfo()
{
	std::vector<std::string>::iterator server = this->_servers.begin();
	for (; server != this->_servers.end(); server++)
	{
		std::cout << *server << std::endl;
	}
}

// ---> Launch private auxiliars -----------------------------------------------------

void Service::_initPollingRequests()
{
	if (poll(this->_pollingRequests.data(), this->_pollingRequests.size(), POLL_TIME_OUT) < 0 && g_shutdown == false)
		throw std::runtime_error(ERR_POLL_FAIL + std::string(std::strerror(errno)));
}

void Service::_pollingManager()
{
	for (size_t i = 0; i < this->_pollingRequests.size(); i++)
	{
		this->_resetInfo();
		this->_getLaunchInfo(i);

		if (this->_hasDataToRead())
			continue;
		if (this->_hasBadRequest())
			continue;
		if (this->_isServerSocket())
			continue;

		this->_hasDataToSend();
	}
}

void Service::_getLaunchInfo(int const i)
{
	this->_tmp.pollID = i;
	this->_tmp.clientID = i - this->_defaultServers;
	this->_tmp.socket = this->_pollingRequests.at(i).fd;
	this->_tmp.mode = this->_pollingRequests.at(i).revents;
	this->_tmp.serverID = this->_getServerIndex();
	this->_tmp.launch = true;
}

int Service::_getServerIndex()
{
	std::vector<std::string>::iterator server = this->_servers.begin();
	for (; server != this->_servers.end(); server++)
	{
		if (server->getSocket() == this->_tmp.socket)
			return server - this->_servers.begin();
	}
	return 0;
}

bool Service::_hasDataToRead()
{
	if (this->_tmp.mode & POLLIN)
	{
		if (this->_isServerSocket())
			this->_acceptConnection();
		else
			this->_readData();
		return true;
	}
	return false;
}

bool Service::_isServerSocket()
{
	std::vector<std::string>::iterator server = this->_servers.begin();
	for (; server != this->_servers.end(); server++)
	{
		if (server->getSocket() == this->_tmp.socket)
			return true;
	}
	return false;
}

void Service::_acceptConnection()
{
	this->_tmp.connectionSocket = accept(this->_tmp.socket, NULL, NULL);

	if (this->_tmp.connectionSocket < 0)
		throw std::runtime_error(ERR_ACCEPT_SOCKET);
	
	fcntl(this->_tmp.connectionSocket, F_SETFL, O_NONBLOCK);
	this->_clients.push_back(Client(this->_servers.at(this->_tmp.serverID), this->_tmp.connectionSocket)); //Criar cliente passando server e socket

	this->_addSocketInPollingRequests();
}

void Service::_readData()
{
	char	buffer[BUFFER_SIZE] = {0};
	int		bytes = recv(this->_tmp.socket, buffer, BUFFER_SIZE, 0);

	if (bytes > 0)
		this->_clients.at(this->_tmp.clientID).appendRequest(buffer, bytes);
	else
		this->_closeConnection(EMPTY_MSG);
}

void Service::_closeConnection(std::string const &msg)
{
	close(this->_tmp.socket);
	this->_pollingRequests.erase(this->_pollingRequests.begin() + this->_tmp.pollID);
	this->_clients.erase(this->_clients.begin() + this->_tmp.clientID);
	if (!msg.empty())
		printMsg(msg, RED);
}

bool Service::_hasBadRequest()
{
	if (this->_tmp.mode & POLLERR)
	{
		this->_closeConnection(POLLERR_MSG);
		return true;
	}
	else if (this->_tmp.mode & POLLHUP)
	{
		this->_closeConnection(POLLHUP_MSG);
		return true;
	}
	else if (this->_tmp.mode & POLLNVAL)
	{
		this->_closeConnection(POLLNVAL_MSG);
		return true;
	}
	return false;
}

void Service::_hasDataToSend()
{
	if (this->_tmp.mode & POLLOUT)
	{
		if (this->_clients.at(this->_tmp.clientID).isTimeout())
		{
			this->_closeConnection(TIMEOUT_MSG);
			return;	
		}

		if (!this->_clients.at(this->_tmp.clientID).isReadyToSend())
			return;
	
		this->_checkRequestedServer();
		this->_clients.at(this->_tmp.clientID).sendResponse();
		this->_closeConnection(EMPTY_MSG);
	}
}

void Service::_checkRequestedServer()
{
	std::string	request = this->_clients.at(this->_tmp.clientID).getRequest();
	std::string	requestedServer;
	size_t 		pos;

	if ((pos = request.find("Host: ")))
	{
		requestedServer = request.substr(pos + std::strlen("Host: "));
		if ((pos = requestedServer.find("\r\n")))
			requestedServer = requestedServer.substr(0, pos);
	}
	else
		return;

	if ((pos = requestedServer.find(":")))
		requestedServer = requestedServer.substr(0, pos);
	
	Server	defaultServer = this->_clients.at(this->_tmp.clientID).getServer();
	if (requestedServer == defaultServer.getServerName())
		return;

	std::vector<std::string>::iterator server = this->_servers.begin();
	for (; server != this->_servers.end(); server++)
	{
		if (requestedServer == server->getServerName() && server->getHost() == defaultServer.getHost())
			this->_clients.at(this->_tmp.clientID).changeServer(*server);
	}
}

// ---> Setup private auxiliars -------------------------------------------------------

void Service::_initAddressParameters()
{
	std::memset(&this->_tmp.parameters, 0, sizeof(this->_tmp.parameters));
	this->_tmp.parameters.ai_family = AF_INET;
	this->_tmp.parameters.ai_socktype = SOCK_STREAM;
	this->_tmp.parameters.ai_protocol = IPPROTO_TCP;
	this->_tmp.address = NULL;
}

void Service::_getSetupInfo(serverVector::iterator server)
{
	server->createSocket();
	this->_tmp.socket = server->getSocket();
	this->_tmp.host = server->getHost();
	this->_tmp.port = server->getPort();
	this->_tmp.launch = false;
}

void Service::_setReuseableAddress()
{
	int active = 1;

	if (setsockopt(this->_tmp.socket, SOL_SOCKET, SO_REUSEADDR, &active, sizeof(int)) < 0)
	{
		this->_resetInfo();
		throw std::runtime_error(ERR_SET_SOCKET + std::string(std::strerror(errno)));
	}
}

void Service::_convertHostToAddress()
{
	if (getaddrinfo(this->_tmp.host.c_str(), this->_tmp.port.c_str(), &this->_tmp.parameters, &this->_tmp.address) != 0)
	{
		this->_resetInfo();
		throw std::runtime_error(ERR_GET_ADDR_INFO + std::string(std::strerror(errno)));
	}
}

void Service::_bindAddressToSocket()
{
	if (this->_tmp.address)
	{
		if (bind(this->_tmp.socket, this->_tmp.address->ai_addr, this->_tmp.address->ai_addrlen) < 0)
		{
			this->_resetInfo();
			throw std::runtime_error(ERR_BIND_SOCKET + std::string(std::strerror(errno)));
		}
	}
}

void Service::_setSocketListening()
{
	if (listen(this->_tmp.socket, MAX_PENDING) < 0)
	{
		this->_resetInfo();
		throw std::runtime_error(ERR_LISTEN_SOCKET + std::string(std::strerror(errno)));
	}
}

// ---> Common private auxiliars -------------------------------------------------------

void Service::_addSocketInPollingRequests()
{
	pollfd request;

	if (this->_tmp.launch == true)
	{
		request.fd = this->_tmp.connectionSocket;
		request.events = POLLIN | POLLOUT;
	}
	else
	{
		request.fd = this->_tmp.socket;
		request.events = POLLIN;
	}
	request.revents = 0;
	this->_pollingRequests.push_back(request);
}

void Service::_resetInfo()
{
	if (this->_tmp.address)
	{
		freeaddrinfo(this->_tmp.address);
		this->_tmp.address = NULL;
	}
	std::memset(&this->_tmp.parameters, 0, sizeof(this->_tmp.parameters));
	this->_tmp.host.clear();
	this->_tmp.port.clear();
	this->_tmp.pollID = 0;
	this->_tmp.clientID = 0;
	this->_tmp.serverID = 0;
	this->_tmp.socket = 0;
	this->_tmp.mode = 0;
	this->_tmp.connectionSocket = 0;
	this->_tmp.launch = false;
}

// ---> Constructor auxiliars -------------------------------------------------------

size_t Service::_countDefaultServers()
{
	serverVector::iterator server = this->_servers.begin();
	size_t count = 0;

	for (; server != this->_servers.end(); server++)
	{
		if (server->getIsDefault() == true)
			count++;
	}
	return count;
}