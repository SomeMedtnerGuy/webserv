/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 12:16:31 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 22:09:02 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# define DEFAULT_PAGE_PATH "./.default/default.html"

# include "HttpMessage.hpp"
# include "utils.hpp"

# include <iostream> //TODO: debug. remove

class HttpResponse: public HttpMessage
{
public:
	typedef unsigned int status_code;

	HttpResponse();
	~HttpResponse();

	void		setStatusCode(status_code statusCode, std::string errorPagePath);
	status_code	getStatusCode(void) const;
	void		setBodyPath(std::string bodyPath);
	std::string	getBodyPath(void) const;
	//FOR DEBUG:
	void	printMessage(void);

private:
	unsigned int	_statusCode;
	std::string		_bodyPath;
};

#endif