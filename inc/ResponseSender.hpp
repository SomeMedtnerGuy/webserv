/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:12:50 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 20:14:35 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_SENDER
# define RESPONSE_SENDER

# include "AMessageHandler.hpp"

class ResponseSender: public AMessageHandler
{
public:
    ResponseSender(HttpRequest& request, HttpResponse& response);
    ~ResponseSender();

    void    handle(void);
};

#endif