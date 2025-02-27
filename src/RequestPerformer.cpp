/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestPerformer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:09:54 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 21:44:07 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestPerformer.hpp"

RequestPerformer::RequestPerformer(HttpRequest& request, HttpResponse& response)
    : AMessageHandler(request, response), _consumeMode(false)
{}
RequestPerformer::~RequestPerformer(){}

size_t    RequestPerformer::perform(Socket::data_container_t data)
{
    std::cerr << "Performer called" << std::endl;
    //TODO
    /* THE FOLLOWING LINES ARE DEBUG */
    std::cerr << std::string(data.begin(), data.end()) << std::endl;
    _setIsDone(true);
    return (data.size());
}

void    RequestPerformer::activateConsumeMode(void)
{
    _setConsumeMode(true);
}

void    RequestPerformer::_setConsumeMode(bool newValue) {_consumeMode = newValue;}
bool    RequestPerformer::_getConsumeMode(void) const {return (_consumeMode);}