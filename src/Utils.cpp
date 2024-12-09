/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 22:28:13 by joamonte          #+#    #+#             */
/*   Updated: 2024/12/09 11:26:10 by joamonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Define.hpp"
#include "Utils.hpp"

void printMsg(std::string const &s, std::string const &color)
{
	if (s == START_MSG)
		std::cout << RESET;
	
	if (s == SHUTDOWN_MSG)
		std::cout << std::endl;

	std::cout << color << "[" << getTime() << "] " << RESET << s << std::endl;
}

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		g_shutdown = true;
		std::cout << YELLOW << "Webserv Engine shutdown complete" << std::endl;
	}
}