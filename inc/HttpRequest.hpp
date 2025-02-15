/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:57:32 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 16:51:43 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "HttpMessage.hpp"

class HttpRequest: public HttpMessage
{
public:
	enum Method {
		GET,
		POST,
		DELETE,
		METHOD_SIZE,
		UNKNOWN
	};
	
	HttpRequest();
	~HttpRequest();

	void		setMethod(Method method);
	Method		getMethod(void) const;
	void		setTarget(std::string target);
	std::string	getTarget(void) const;

private:
	Method		_method;
	std::string	_target;
};

#endif