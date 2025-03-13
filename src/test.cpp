/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:44:57 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/13 16:31:13 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "Socket.hpp"
#include "Client.hpp"
#include "Webserv.hpp"
#include <fcntl.h>

#include <signal.h>

int g_shutdown;

// void signalHandler(int signum)
// {
// 	if (signum == SIGINT) {
// 		g_shutdown = true;
// 	}
// }

// int main(int argc, char** argv)
// {
//     if (argc > 2) {
//         std::cerr << WRONG_USAGE_MSG << std::endl;
//         return (1);
//     }
//     signal(SIGPIPE, SIG_IGN);
//     signal(SIGINT, signalHandler);
//     g_shutdown = false;

//     Webserv webserv(argc, argv);
//     webserv.setup();
//     webserv.run();
//     std::cout << "Byeeeeeeeee" << std::endl;
// }

int main(int argc, char** argv)
{

    try
    {
        ConfigFile conf(argc, argv);
        Location a = conf.getServer("localhost", 1234).getLocation()[0];
        Server s = conf.getServer("localhost", 1234);
        ServerSettings ss(conf, 1234);
        
        ss.setServer("localhost");
        std::cout << ss.isCgiExtension(".bla") << std::endl;
        std::cout << ss.cgiExtensionHasASpecifcScript(".bla") << std::endl;
        ss.setLocation("/");
        std::cout << ss.isCgiExtension(".py") << std::endl;
        std::cout << ss.cgiExtensionHasASpecifcScript(".py") << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
}