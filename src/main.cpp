/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 14:20:21 by joamonte          #+#    #+#             */
/*   Updated: 2024/12/08 23:08:00 by joamonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Service.hpp"

int	main(int ac, char **av)
{
	try
	{
		Service	webserv(ac, av);
		
		webserv.setup();
		webserv.launch();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return (EXIT_FAILURE);
	}
}
