/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:13:08 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 22:53:51 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseSender.hpp"

ResponseSender::ResponseSender(HttpRequest& request, HttpResponse& response)
    : AMessageHandler(request, response)
{}
ResponseSender::~ResponseSender(){}

Socket::data_container_t    ResponseSender::getMessageToSend(size_t byteLimit)
{
    std::cerr << "Sender called" << std::endl;
    //TODO
    /* THE FOLLOWING LINES ARE DEBUG */
    _setIsDone(true);
    (void)byteLimit;
    Socket::data_container_t    output(0);
    output.insert(output.end(), 'l');
    output.insert(output.end(), 'o');
    output.insert(output.end(), 'l');
    std::cerr << "loles: " << output.size() << std::endl;
    return (output);
}