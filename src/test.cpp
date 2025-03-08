/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:44:57 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/08 22:08:06 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "Socket.hpp"
#include "Client.hpp"
#include <fcntl.h>

#include <signal.h>

int main(int argc, char** argv)
{
    // User should call webserv with no arguments or one config file only
    if (argc > 2) {
        std::cerr << WRONG_USAGE_MSG << std::endl;
        return (1);
    }
    // This makes sure that the i/o operations do not get fucked by clients unexpectedly closing connection.
    // Now, recv/send return -1 instead, which should make the server clean up the client and stay alive
    signal(SIGPIPE, SIG_IGN);
    
    ConfigFile  configFile(argc, argv);
    
    typedef std::vector<struct pollfd> sock_vector;
    typedef std::vector<int> port_vector;
    typedef std::vector<Client> client_vector;

    sock_vector listenSockets;
    port_vector ports = configFile.getPorts();
    
    for (port_vector::iterator it = ports.begin(); it != ports.end(); it++) {
        int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(listenSocket, F_SETFL, O_NONBLOCK);
        int option = 1;
        setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                    &option, sizeof(option));
        struct sockaddr_in  serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(*it);
        int bindStatus = bind(listenSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if (bindStatus < 0){
            std::cerr << BINDING_ERROR_MSG << std::endl;
            return (1);
        }
        listen(listenSocket, 1);
        struct pollfd   listenPollfd;
        listenPollfd.fd = listenSocket;
        listenPollfd.events = POLLIN;
        listenSockets.push_back(listenPollfd);
    }

    
    //Prepare sockets and listen for request for connections.
    //At the moment a single hardcoded socket is used.
    /*int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                &option, sizeof(option));
    struct sockaddr_in  serv_addr;
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(1234); // hardcode right here
    int bindStatus = bind(listenSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (bindStatus < 0){
        std::cerr << BINDING_ERROR_MSG << std::endl;
        return (1);
    }
    listen(listenSocket, 1); //This blocks while waiting*/

    // Accept connection
    /*struct pollfd       fds[1];
    struct sockaddr_in  cli_addr;
    socklen_t           cli_len = sizeof(cli_addr);
    fds[0].fd = accept(listenSocket, (struct sockaddr*)&cli_addr, &cli_len);
    fds[0].events = POLLIN | POLLOUT; // At the moment only listen for buffer activity
    std::cout << "Connection accepted!" << std::endl;*/
    
    client_vector   clients;
    try {
        while (true) {
            //std::cerr << "current number of clients: " << clients.size() << std::endl;
            poll(listenSockets.data(), listenSockets.size(), 200);
            for (client_vector::reverse_iterator rclient = clients.rbegin(); rclient != clients.rend() ; rclient++) {
                client_vector::iterator client = rclient.base() - 1; //The formula to convert rev to reg iterator
                int clientIndex = client - clients.begin();
                //std::cerr << "Client being handled: " << clientIndex << std::endl;
                rclient->updateSocketFlags(listenSockets[ports.size() + clientIndex].revents);
                rclient->handle();
                std::cerr << clients.size() << std::endl;
                if (rclient->shouldCloseConnection()) {
                    std::cerr << "Client being deleted: " << clientIndex << std::endl;
                    clients.erase(client); 
                    close(listenSockets[ports.size() + clientIndex].fd);
                    listenSockets.erase(listenSockets.begin() + ports.size() + clientIndex);
                }
            }
            for (size_t i = 0; i < ports.size(); i++) {
                if (listenSockets[i].revents & POLLIN) {
                    std::cerr << "Client being created from listen socket: " << i << std::endl;
                    struct sockaddr_in  cli_addr;
                    socklen_t           cli_len = sizeof(cli_addr);
                    struct pollfd       cliSock;
                    cliSock.fd = accept(listenSockets[i].fd, (struct sockaddr*)&cli_addr, &cli_len);
                    cliSock.events = POLLIN | POLLOUT;
                    listenSockets.push_back(cliSock);
                    Client  client(cliSock.fd, configFile);
                    clients.push_back(client);
                    listenSockets[i].revents = 0;
                }
            }
        }  
    } catch (std::exception& e) {
        std::cerr << "some shit went wrong." << std::endl;
    }
    
    /*try {
        Client  client(0, fds[0], configFile);
        while (true) {
            //Somehow 
            poll(listenSockets.data(), 1, -1);
            
            for (client in clients)
            {
                client.handle();
            
                if (client.shouldCloseConnection()) {
                    //cleanClient();
                    break;
                };
            }

            //for listenSocket in listenSockets 
            //    accept connections if the fd has POLLIN active
        }
    } catch (std::exception& e) {
        std::cerr << "some shit went wrong." << std::endl;
    }*/
    std::cout << "Program done!" << std::endl;
}
