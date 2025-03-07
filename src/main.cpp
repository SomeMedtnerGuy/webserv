/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 14:20:21 by joamonte          #+#    #+#             */
/*   Updated: 2025/03/07 11:24:50 by joamonte         ###   ########.fr       */
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
