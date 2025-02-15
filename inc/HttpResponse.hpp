/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 12:16:31 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 17:18:52 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include "HttpMessage.hpp"

class HttpResponse: public HttpMessage
{
public:
	HttpResponse();
	~HttpResponse();

private:
	unsigned int	_statusCode;
	std::string		_bodyPath;
};

#endif