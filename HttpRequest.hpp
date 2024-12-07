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

# include <iostream> // for std::cout and std::endl
# include <string> // for std::string
# include <cstring> // for std::memset
# include <unistd.h> // for sleep
# include <sys/socket.h> // for recv


# include "RequestLineParser.hpp"
# include "HeadersParser.hpp"
# include "BodyParser.hpp"


# define BUFFER_SIZE 20

class	HttpRequest
{
	private:
		enum State 
		{
			REQUEST_LINE_PARSING,
			HEADERS_PARSING,
			BODY_PARSING,
			FINISHED_PARSER
		};
		
		
		//RequestLineParser			_rlp;
		//HeadersParser				_hp;
		//BodyParser					_bp;

		State						_currentState;
		char						_buffer[BUFFER_SIZE];
		std::string					_currentRequest;

		int							_sockfd;
		bool						_eof;

		HttpRequest();
		bool	_isBuffValid(void);

	public:
		HttpRequest(int sockfd);
		HttpRequest(const HttpRequest& other);
		HttpRequest&	operator=(const HttpRequest& other);
		~HttpRequest();

		void	parseMoreRequest(void);
		bool	eof(void);

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
