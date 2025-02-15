/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 12:04:51 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 12:08:31 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_MESSAGE_HPP
# define HTTP_MESSAGE_HPP

# include <map>
# include <string>

class HttpMessage
{
public:
	typedef std::map<std::string, std::string>	headers_dict;
	
	HttpMessage();
	virtual ~HttpMessage();
protected:
	headers_dict	_headers;
};

#endif