/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:44:57 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/04 18:54:19 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "Socket.hpp"
#include "Client.hpp"
#include "PollManager.hpp"

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
    
    
    //Prepare sockets and listen for request for connections.
    //At the moment a single hardcoded socket is used.
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
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
    listen(listenSocket, 1); //This blocks while waiting

    // Accept connection
    struct pollfd       fds[1];
    struct sockaddr_in  cli_addr;
    socklen_t           cli_len = sizeof(cli_addr);
    fds[0].fd = accept(listenSocket, (struct sockaddr*)&cli_addr, &cli_len);
    fds[0].events = POLLIN | POLLOUT; // At the moment only listen for buffer activity
    std::cout << "Connection accepted!" << std::endl;
    
    try {
        Socket  socket(fds[0]);
        Client  client(socket, configFile);
        while (true) {
            poll(fds, 1, -1);
            //std::cerr << "Loop" << std::endl;
            // The following must be changed for a while loop through the clients
            
            client.handle();
            if (client.shouldCloseConnection()) {
                break;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "some shit went wrong." << std::endl;
    }
    std::cout << "Program done!" << std::endl;
}