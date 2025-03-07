/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:19:35 by joamonte          #+#    #+#             */
/*   Updated: 2025/03/07 11:47:52 by joamonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "Define.hpp"
#include "ConfigFile.hpp"
#include "ServerSettings.hpp"

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

class Service
{
	private:
		clientVector	_clients;
		pollfdVector	_pollingRequests;
		serviceInfo		_tmp;
		ConfigFile		_parsing;

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