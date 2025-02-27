/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:12:50 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 21:39:12 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_SENDER
# define RESPONSE_SENDER

# include "AMessageHandler.hpp"
# include "Socket.hpp"

class ResponseSender: public AMessageHandler
{
public:
    ResponseSender(HttpRequest& request, HttpResponse& response);
    ~ResponseSender();

    Socket::data_container_t    getMessageToSend(size_t byteLimit);
};

#endif