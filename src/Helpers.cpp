/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helpers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:58:34 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/03 17:04:56 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Helpers.hpp"

Helpers::Helpers(){}
Helpers::~Helpers(){}

int Helpers::getFileLength(std::string filename)
{
    std::ifstream       file(filename.c_str()); //TODO: Protect this
    std::streampos      length;
    std::streampos      pos(file.tellg());
    
    file.seekg(pos, std::ios::end);
    length = file.tellg();
    return (static_cast<int>(length));
}

int		Helpers::getPortFromSocket(int sockfd)
{
	struct sockaddr_in	addr;
	socklen_t	len = sizeof(addr);
	getsockname(sockfd, (struct sockaddr*)&addr, &len);
	return (ntohs(addr.sin_port));
}
