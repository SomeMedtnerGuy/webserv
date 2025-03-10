/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:49:01 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/10 14:50:54 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <fcntl.h>
# include "Client.hpp"
# include "main.h"

class Webserv
{
public:
    Webserv(int argc, char** argv);
    ~Webserv();
    
    void    setup(void);
    void    run(void);

private:
    typedef std::vector<struct pollfd> sock_vector;
    typedef std::vector<int> port_vector;
    typedef std::vector<Client> client_vector;
    
    ConfigFile      _configFile;
    const size_t    _portsAm;
    sock_vector     _pollSockets;
    client_vector   _clients;

    void    _takeCareOfClientSockets(void);
    void    _takeCareOfListenSockets(void);
};

#endif