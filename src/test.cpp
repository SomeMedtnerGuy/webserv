/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 20:23:43 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/22 16:43:37 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "RequestHandler.hpp"
#include "../inc/ConfigFile.hpp"
#include "../inc/RequestHandler.hpp"

#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

int main(int argc, char **argv)
{
	ConfigFile configFile(argc, argv);
	int			insockfd, outsockfd;
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
	clilen = sizeof(cli_addr);
	outsockfd = accept(insockfd, (struct sockaddr *)&cli_addr, &clilen);
	std::cout << "connection accepted" << std::endl;	
	int flags = fcntl(outsockfd, F_GETFL, 0);
	fcntl(outsockfd, F_SETFL, flags | O_NONBLOCK);
	try
	{
		RequestHandler	rh(outsockfd, configFile);
		while (true)
		{
			try {
				rh.handleRequest();
				if (rh.isRequestHandled())
				{
					close(outsockfd);
					break ;
				}
			} catch (const RequestHandler::RecvSendError& e){
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
