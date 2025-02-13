/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:19:35 by joamonte          #+#    #+#             */
/*   Updated: 2025/02/13 12:49:51 by joamonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "Define.hpp"
#include "ConfigFile.hpp"
//#include "Parser.hpp"
//#include "Client.hpp"

struct serviceInfo
{
	addrinfo	parameters;
	addrinfo	*address;
	std::string	host;
	std::string	port;
	int			socket;
	int			clientID;
	int			serverID;
	int			connectionSocket;
	int			pollID;
	short		mode;
	bool		launch;
};

typedef std::vector<pollfd>	pollfdVector;
typedef std::vector<Client>	clientVector;
typedef std::vector<Server>	serverVector;

class Service
{
	private:
		serverVector	_servers;
		clientVector	_clients;
		pollfdVector	_pollingRequests;
		size_t			_defaultServers;
		serviceInfo		_tmp;

		// Constructor auxiliar
		size_t	_countDefaultServers();

		//Setup auxiliars
		void	_initAddressParameters();
		void	_getSetupInfo(serverVector::iterator server);
		void	_setReuseableAddress();
		void	_convertHostToAddress();
		void	_bindAddressToSocket();
		void	_setSocketListening();

		//Launch auxiliars
		void	_initPollingRequests();
		void	_pollingManager();
		void	_getLaunchInfo(int const i);
		int		_getServerIndex();
		bool	_hasDataToRead();
		bool	_isServerSocket();
		void	_acceptConnection();
		void	_readData();
		void	_closeConnection(std::string const &msg);
		bool	_hasBadRequest();
		void	_hasDataToSend();
		void	_checkRequestedServer();

		void	_addSocketInPollingRequests();
		void	_resetInfo();

		Service();

	public:
		Service(int ac, char **av);
		~Service();

		void setup();
		void launch();
		void printServersInfo();
};

#endif