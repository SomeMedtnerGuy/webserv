/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 20:23:43 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/12 16:28:11 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	// ConfigFile configFile(argc, argv); /* need to be in a try and catch */
	// int			insockfd, outsockfd;
	// socklen_t		clilen;
	// struct sockaddr_in	serv_addr, cli_addr;

	// (void)argv;
	// if (argc < 1)
	// 	return (1);
	// insockfd = socket(AF_INET, SOCK_STREAM, 0);
	// int	option = 1;
	// setsockopt(insockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
	// serv_addr.sin_family = AF_INET;
	// serv_addr.sin_addr.s_addr = INADDR_ANY;
	// serv_addr.sin_port = htons(1234);
	// if (bind(insockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	// {
	// 	std::cerr << "Bind of socket to address failed." << std::endl;
	// 	return (1);
	// }
	// listen(insockfd, 1);
	// clilen = sizeof(cli_addr);
	// outsockfd = accept(insockfd, (struct sockaddr *)&cli_addr, &clilen);
	// std::cout << "connection accepted" << std::endl;	
	// int flags = fcntl(outsockfd, F_GETFL, 0);
	// fcntl(outsockfd, F_SETFL, flags | O_NONBLOCK);
	// try
	// {
	// 	RequestHandler	rh(outsockfd, configFile);
	// 	while (true)
	// 	{
	// 		try {
	// 			rh.handleRequest();
	// 			if (rh.isRequestHandled())
	// 			{
	// 				close(outsockfd);
	// 				break ;
	// 			}
	// 		} catch (const RequestHandler::RecvSendError& e){
	// 			continue ;
	// 		}	
	// 	}
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << "Fuck. " << e.what() << std::endl;
	// }
	// std::cerr << "End of program" << std::endl;

	try
	{
		ConfigFile parsing(argc, argv);

		//std::cout << parsing.getServer("hostname").getLocation()[0].getAllowMethods("GET") << std::endl;

		std::cout << "Server Settings default:" << std::endl;

		ServerSettings serversettings(parsing);
		std::cout << "Server name " << serversettings.getServerName() << std::endl;
		std::cout << "Root " << serversettings.getRoot() << std::endl;
		std::cout << "Index " << serversettings.getIndex() << std::endl;
		std::cout << "Error page 404 " << serversettings.getErrorPage(404) << std::endl;
		std::cout << "Return code " << serversettings.getReturnCode() << std::endl;
		std::cout << "Return URL " << serversettings.getReturnURL() << std::endl;
		std::cout << "Client body size " << serversettings.getClientBodySize() << std::endl;
		std::cout << "Auto index " << serversettings.getAutoIndex() << std::endl;
		std::cout << "GET " << serversettings.getAllowMethod(GET) << std::endl;
		std::cout << "POST " << serversettings.getAllowMethod(POST) << std::endl;
		std::cout << "DELETE " << serversettings.getAllowMethod(DELETE) << std::endl;
		std::cout << "UNKNOWN " << serversettings.getAllowMethod(UNKNOWN) << std::endl;

		std::cout << std::endl;
		std::cout << "Set server localhost:" << std::endl;
		serversettings.setServer("localhost");

		std::cout << "Server name " << serversettings.getServerName() << std::endl;
		std::cout << "Root " << serversettings.getRoot() << std::endl;
		std::cout << "Index " << serversettings.getIndex() << std::endl;
		std::cout << "Error page 404 " << serversettings.getErrorPage(404) << std::endl;
		std::cout << "Return code " << serversettings.getReturnCode() << std::endl;
		std::cout << "Return URL " << serversettings.getReturnURL() << std::endl;
		std::cout << "Client body size " << serversettings.getClientBodySize() << std::endl;
		std::cout << "Auto index " << serversettings.getAutoIndex() << std::endl;
		std::cout << "GET " << serversettings.getAllowMethod(GET) << std::endl;
		std::cout << "POST " << serversettings.getAllowMethod(POST) << std::endl;
		std::cout << "DELETE " << serversettings.getAllowMethod(DELETE) << std::endl;
		std::cout << "UNKNOWN " << serversettings.getAllowMethod(UNKNOWN) << std::endl;

		std::cout << std::endl;
		std::cout << "Set location /test/tchau:" << std::endl;
		serversettings.setLocation("/test/tchau");

		std::cout << "Server name " << serversettings.getServerName() << std::endl;
		std::cout << "Root " << serversettings.getRoot() << std::endl;
		std::cout << "Index " << serversettings.getIndex() << std::endl;
		std::cout << "Error page 404 " << serversettings.getErrorPage(404) << std::endl;
		std::cout << "Return code " << serversettings.getReturnCode() << std::endl;
		std::cout << "Return URL " << serversettings.getReturnURL() << std::endl;
		std::cout << "Client body size " << serversettings.getClientBodySize() << std::endl;
		std::cout << "Auto index " << serversettings.getAutoIndex() << std::endl;
		std::cout << "GET " << serversettings.getAllowMethod(GET) << std::endl;
		std::cout << "POST " << serversettings.getAllowMethod(POST) << std::endl;
		std::cout << "DELETE " << serversettings.getAllowMethod(DELETE) << std::endl;
		std::cout << "UNKNOWN " << serversettings.getAllowMethod(UNKNOWN) << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	// ConfigFile parsing(argc, argv);

	// std::cout << parsing.getServer("localhost").getLocation()[0].getSpecificPath() << " - " << parsing.getServer("localhost").getLocation()[0].getAutoIndex() << std::endl;
	// std::cout << parsing.getServer("localhost").getLocation()[1].getSpecificPath() << " - " << parsing.getServer("localhost").getLocation()[1].getAutoIndex() << std::endl;

	// ServerSettings serversettings(parsing);
	// serversettings.setServer("localhost");
	// serversettings.setLocation("/");
	
	// std::cout << "Location " << serversettings.getRoot() << std::endl;
	// std::cout << "Auto index " << serversettings.getAutoIndex() << std::endl;
	// std::cout << "Index " << serversettings.getIndex() << std::endl;
	
}
