/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:56:27 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 23:09:07 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser(HttpRequest& request, HttpResponse& response)
    : AMessageHandler(request, response)
{}
RequestParser::~RequestParser(){}

size_t    RequestParser::parse(Socket::data_container_t data)
{
    std::cerr << "Parsing called" << std::endl;
    //TODO
    /* THE FOLLOWING TWO LINES ARE DEBUG */
    std::cerr << std::string(data.begin(), data.end()) << std::endl;
    if (data.size())
        _setIsDone(true);
    return (data.size());
}