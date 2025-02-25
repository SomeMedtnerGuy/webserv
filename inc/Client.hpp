/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:19:05 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/25 18:42:16 by ndo-vale         ###   ########.fr       */
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
    Client(Socket& socket);
    ~Client();

    void    handle(void); //To be called in each iteration (between poll() calls)
    bool    shouldCloseConnection(void); // Check after each time handleClient() is called
private:
    Socket&         _socket;
    RequestHandler* _activeRequest;
    bool            _closeConnection;
};

#endif