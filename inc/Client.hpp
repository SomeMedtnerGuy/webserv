/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:19:05 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 13:47:03 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <memory>

# include "Socket.hpp"
# include "RequestHandler.hpp"

class   Client
{
public:
    Client(Socket& socket, ConfigFile& configFile);
    ~Client();

    void    handle(void); //To be called in each iteration (between poll() calls)
    bool    shouldCloseConnection(void) const; // Check after each time handle() is called
private:
    Socket&         _socket;
    ConfigFile&     _configFile;
    RequestHandler* _activeRequest;
    bool            _closeConnection;
        void    _setCloseConnection(bool closeConnection);
        bool    _getCloseConnection(void) const;
    
    bool    _isNewRequestRequired(void) const;
};

#endif