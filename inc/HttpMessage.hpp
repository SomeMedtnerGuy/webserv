/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 10:09:39 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/25 10:17:09 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_MESSAGE_HPP
# define HTTP_MESSAGE_HPP

# define BUFFER_SIZE 8000
# define MAX_HEADER_SIZE (BUFFER_SIZE * 4)

# include <cstring> // For memset
# include <sys/socket.h> // For recv and send
# include <netinet/in.h> // For stuff in getPortFromSocket
# include <iostream> // For cout and endl
# include <string>
# include <map>

class HttpMessage
{
public:
	typedef std::map<std::string, std::string> headers_dict;

	/* Public canonical methods */
	HttpMessage();
	virtual ~HttpMessage();

	/* Setters */
	void	addHeader(std::string name, std::string value);
	/* Getters */
	const headers_dict&	getHeaders(void) const;

protected:
	headers_dict	_headers;

	HttpMessage(const HttpMessage& other);
	HttpMessage&	operator=(const HttpMessage& other);

};

#endif