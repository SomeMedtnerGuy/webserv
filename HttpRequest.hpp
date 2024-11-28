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

# include <cstring>
# include <string>
# include <array>
# include <iostream>

//TO MOVE TO MAIN HEADER
enum	Method
{
	INVALID = -1,
	GET,
	POST,
	DELETE,
	METHODS_SIZE
};


class	HttpRequest
{
	private:
		const std::string	_methods[3];

		char*	_request;
		char*	_requestPtr;
	
		Method	_method;
		

		HttpRequest();

		void	_parseMethod(void);
		void	_parseTarget(void);
	public:
		HttpRequest(char* request);
		HttpRequest(const HttpRequest& other);
		HttpRequest&	operator=(const HttpRequest& other);
		~HttpRequest();

		void	parseRequest(void);


		//Exceptions
		class HttpRequestException: public std::exception
		{
			private:
				std::string	_message;
			public:
				HttpRequestException(std::string msg);
				~HttpRequestException() throw();
				const char* what() const throw();
		};

};

#endif
