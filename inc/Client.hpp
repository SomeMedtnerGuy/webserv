/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:19:05 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/08 09:08:11 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <memory>

# include "Socket.hpp"
# include "ConfigFile.hpp"
# include "RequestManager.hpp"
# include "utils.hpp"

class   Client
{
public:
    Client(int id, struct pollfd& sockfd, ConfigFile& configFile);
    ~Client();

    void    handle(void); //To be called in each iteration (between poll() calls)
    int     getId(void) const;
    bool    shouldCloseConnection(void) const; // Check after each time handle() is called
private:
    const int       _id;
    Socket          _socket;
    ConfigFile&     _configFile;
    RequestManager* _activeRequest;

    bool            _closeConnection;
        void    _setCloseConnection(bool closeConnection);
        bool    _getCloseConnection(void) const;
    
    bool    _isNewRequestRequired(void) const;
    
    const int   _timeoutTime;
    long long   _lastActionTime;
};

#endif