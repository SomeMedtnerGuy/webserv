/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:12:50 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 17:47:58 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_SENDER
# define RESPONSE_SENDER

# define DELIMITOR "\r\n"

# include "AMessageHandler.hpp"
# include "Socket.hpp"
# include "CGIHandler.hpp"

class ResponseSender: public AMessageHandler
{
public:
    typedef Socket::data_container_t data_t;

    ResponseSender(HttpRequest& request, HttpResponse& response);
    ~ResponseSender();

    data_t    getMessageToSend(size_t byteLimit);

private:
    std::ifstream   _file;
    bool            _headersSent; 

    std::string _generateResponseHeader(void);
};

#endif