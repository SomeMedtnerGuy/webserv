/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 20:56:17 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/11/27 21:09:53 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

#include <cstring>
#include <string>
#include <array>

//TO MOVE TO MAIN HEADER
enum	Method
{
	GET,
	POST,
	DELETE
};


class	HttpRequest
{
	private:
		std::string	_methods[3];
		Method	_method;

		HttpRequest();
	public:
		HttpRequest(char* request);
		HttpRequest(const HttpRequest& other);
		HttpRequest&	operator=(const HttpRequest& other);
		~HttpRequest();
};

#endif
