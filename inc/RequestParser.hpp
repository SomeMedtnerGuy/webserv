/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:37:12 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 21:40:54 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER
# define REQUEST_PARSER

# include "AMessageHandler.hpp"
# include "Socket.hpp"

class RequestParser: public AMessageHandler
{
public:
    RequestParser(HttpRequest& request, HttpResponse& response);
    ~RequestParser();

    size_t    parse(Socket::data_container_t data);
};

#endif