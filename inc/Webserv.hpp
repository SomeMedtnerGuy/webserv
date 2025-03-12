/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:49:01 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/12 13:21:37 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <deque>
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
    typedef std::deque<Client> client_deque;
    
    ConfigFile      _configFile;
    const size_t    _portsAm;
    sock_vector     _pollSockets;
    client_deque   _clients;

    void    _takeCareOfClientSockets(void);
    void    _takeCareOfListenSockets(void);
};

#endif