/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:16:04 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/12/10 19:40:45 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "utils.hpp"

class Location {
private:
	std::string _specificPath;
	std::vector<std::string> _allowMethods;
	bool _autoindex;
	std::string _index;
	std::string _return;	

public:
	Location();
	Location(const Location& src);
	Location& operator=(const Location& src);
	~Location();
};

#endif // LOCATION_HPP