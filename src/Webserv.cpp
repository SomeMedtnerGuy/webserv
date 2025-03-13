/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:51:06 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/13 11:37:29 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(int argc, char** argv)
    : _configFile(argc, argv), _portsAm(_configFile.getPorts().size()){
    }
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
        listenPollfd.revents = 0;
        _pollSockets.push_back(listenPollfd);
    }
}

void    Webserv::run(void)
{
    try {
        while (g_shutdown == false) {
            poll(_pollSockets.data(), _pollSockets.size(), 200);
            _takeCareOfClientSockets();
            _takeCareOfListenSockets();
        } 
    } catch (std::exception& e) {
        std::cerr << "Some shit went wrong. " << e.what() << std::endl;
    }
}

/* PRIVATE */
void    Webserv::_takeCareOfClientSockets(void)
{
    for (client_deque::reverse_iterator rclient = _clients.rbegin(); rclient != _clients.rend() ; rclient++) {
        client_deque::iterator client = rclient.base() - 1; //The formula to convert rev to reg iterator
        // Client index is relative to total poll sockets, not just client sockets (includes listen sockets)
        int clientIndex = _portsAm + (client - _clients.begin());
        rclient->updateSocketFlags(_pollSockets[clientIndex].revents);
        rclient->handle();
        if (rclient->shouldCloseConnection()) {
            _clients.erase(client); 
            close(_pollSockets[clientIndex].fd);
            _pollSockets.erase(_pollSockets.begin() + clientIndex);
        }
    }
}

void    Webserv::_takeCareOfListenSockets()
{
    for (size_t i = 0; i < _portsAm; i++) { //Only go through the listen sockets from the pollSockets (the first _portsAm amout of sockets in that vector)
        if (_pollSockets[i].revents & POLLIN) {
            struct sockaddr_in  cli_addr;
            socklen_t           cli_len = sizeof(cli_addr);
            struct pollfd       cliSock;
            cliSock.fd = accept(_pollSockets[i].fd, (struct sockaddr*)&cli_addr, &cli_len);
            cliSock.events = POLLIN | POLLOUT;
            cliSock.revents = 0;
            _pollSockets.push_back(cliSock);
            
            Client  client(cliSock.fd, _configFile);
            _clients.push_back(client);
        }
    } 
}
