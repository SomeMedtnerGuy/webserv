/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 20:23:43 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/21 20:12:52 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpRequest.hpp"

#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h>
#include <cstring>

#include <unistd.h>

int main(int argc, char **argv)
{
    int			insockfd, outsockfd;
	socklen_t		clilen;
	struct sockaddr_in	serv_addr, cli_addr;
	//char			buffer[2048];

	(void)argv;
	if (argc < 1)
		return (1);
	insockfd = socket(AF_INET, SOCK_STREAM, 0);
	int	option = 1;
	setsockopt(insockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
	//memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//struct hostent *host = gethostbyname("jax");
	//memcpy(&serv_addr.sin_addr.s_addr, host->h_addr, host->h_length);
	serv_addr.sin_port = htons(8763);
	if (bind(insockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "FUCK" << std::endl;
		return (1);
	}
	listen(insockfd, 1);
	clilen = sizeof(cli_addr);
	outsockfd = accept(insockfd, (struct sockaddr *)&cli_addr, &clilen);
	
    int flags = fcntl(outsockfd, F_GETFL, 0);
    fcntl(outsockfd, F_SETFL, flags | O_NONBLOCK);

	
    try
    {
        HttpRequest httpRequest(outsockfd);
		while (true)
		{
			//std::cerr << "Fuck everything" << std::endl;
			httpRequest.handleRequest();
			if (httpRequest.eof())
				break ;
			else if (httpRequest.recvStatus() <= 0)
				continue ;
			
			
		}
		std::cout << "DONE OMG" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fuck. " << e.what() << std::endl;
    }
	std::cerr << "what the fuck, seriously" << std::endl;
}