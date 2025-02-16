/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 20:23:43 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 23:09:20 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../inc/ConfigFile.hpp"
#include "../inc/RequestManager.hpp"
#include "Socket.hpp"
#include "ConfigFile.hpp"

#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

int main(int argc, char **argv)
{
	ConfigFile configFile(argc, argv);
	int			insockfd;
	socklen_t		clilen;
	struct sockaddr_in	serv_addr, cli_addr;

	(void)argv;
	if (argc < 1)
		return (1);
	insockfd = socket(AF_INET, SOCK_STREAM, 0);
	int	option = 1;
	setsockopt(insockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(1234);
	if (bind(insockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "Bind of socket to address failed." << std::endl;
		return (1);
	}
	listen(insockfd, 1);

	struct pollfd fds[1];
	clilen = sizeof(cli_addr);
	fds[0].fd = accept(insockfd, (struct sockaddr *)&cli_addr, &clilen);
	fds[0].events = POLLIN | POLLOUT;
	std::cout << "connection accepted" << std::endl;	
	
	//int flags = fcntl(outsockfd, F_GETFL, 0);
	//fcntl(outsockfd, F_SETFL, flags | O_NONBLOCK);
	Socket	socket(fds[0].fd);
	try
	{
		//TODO: Put here a wrapper for the socket (ex. connection) which holds an auto_ptr for a request each time there is a new one
		RequestManager	rh(socket, configFile);
		while (true)
		{
			int ret = poll(fds, 1, -1);
			if (ret > 0 && (fds[0].revents & POLLOUT))
                socket.setCanWrite(true);
			if (ret > 0 && (fds[0].revents & POLLIN))
				socket.setCanRead(true);
			try {
				rh.handleMoreRequest();
				if (rh.isHandlingComplete())
				{
					close(fds[0].fd);
					break ;
				}
			} catch (std::exception& e){
				std::cerr << "some shit went wrong" << std::endl;
				continue ;
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Fuck. " << e.what() << std::endl;
	}
	std::cerr << "End of program" << std::endl;
}
