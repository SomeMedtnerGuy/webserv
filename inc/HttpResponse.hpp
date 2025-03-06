/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 12:16:31 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 15:19:36 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# define DEFAULT_PAGE_PATH "./.default/default.html"

# include "HttpMessage.hpp"
# include "ServerSettings.hpp"
# include "utils.hpp"

# include <iostream> //TODO: debug. remove

class HttpResponse: public HttpMessage
{
public:
	typedef int code_t;

	HttpResponse(ServerSettings& serverSettings);
	~HttpResponse();

	void		setStatusCode(code_t statusCode);
	code_t		getStatusCode(void) const;
	void		setBodyPath(std::string bodyPath);
	std::string	getBodyPath(void) const;
	//FOR DEBUG:
	void	printMessage(void);

private:
	typedef	std::map<std::string, std::string> header_map;
	static const header_map	_createFileTypeMap(void);
	static const header_map	_fileTypeMap;

	ServerSettings&	_serverSettings;
	unsigned int	_statusCode;
	std::string		_bodyPath;
	

	
};

#endif