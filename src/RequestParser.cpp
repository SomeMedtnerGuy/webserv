/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:56:27 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 20:08:32 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser(HttpRequest& request, HttpResponse& response)
    : AMessageHandler(request, response)
{}
RequestParser::~RequestParser(){}

void    RequestParser::handle()
{
    //TODO
}