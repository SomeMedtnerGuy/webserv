/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:51:06 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/08 23:19:30 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(int argc, char** argv)
    : _configFile(argc, argv), _portsAm(_configFile.getPorts().size()){}
Webserv::~Webserv(){}

void    Webserv::setup(void)
{
    // Create a listening socket per port
    port_vector ports = _configFile.getPorts();
    for (port_vector::const_iterator it = ports.begin(); it != ports.end(); it++) {
        // Create and setup socket
        int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(listenSocket, F_SETFL, O_NONBLOCK);
        int option = 1;
        setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                    &option, sizeof(option));
        // Create and setup socket address
        struct sockaddr_in  serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(*it); //Add the current port of the iteration
        
        int bindStatus = bind(listenSocket, (struct sockaddr*)&serv_addr,
                                sizeof(serv_addr));
        if (bindStatus < 0){
            throw (std::runtime_error(BINDING_ERROR_MSG));
        }
        listen(listenSocket, 1);
        
        // Setup the socket to be handled by poll()
        struct pollfd   listenPollfd;
        listenPollfd.fd = listenSocket;
        listenPollfd.events = POLLIN;
        _pollSockets.push_back(listenPollfd);
    }
}

void    Webserv::run(void)
{
    //TODO: add the loop present in main()
}