/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 12:04:51 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/01 13:45:50 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_MESSAGE_HPP
# define HTTP_MESSAGE_HPP

# include <map>
# include <string>
# include <stdexcept> // for out_of_range exception

class HttpMessage
{
public:
	typedef std::map<std::string, std::string>	headers_dict;
	
	HttpMessage();
	virtual ~HttpMessage();

	void				addHeader(std::string fieldName, std::string fieldValue);
	const headers_dict&	getHeaders(void) const;

	void				setHeadersSize(size_t newValue);
	size_t				getHeadersSize(void) const;		
protected:
	headers_dict	_headers;
	size_t			_headersSize;
};

#endif