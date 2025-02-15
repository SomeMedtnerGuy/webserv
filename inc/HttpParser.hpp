/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:23:44 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 18:58:11 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_PARSER_HPP
# define HTTP_PARSER_HPP

#include <string>
#include <HttpRequest.hpp>

class HttpParser
{
public:
	typedef unsigned int code;
	static code	httpSanitizer(std::string requestLine);
	static code	parseRequestLine(HttpRequest& request, std::string requestLine);
	static code parseHeaderField(HttpRequest& request, std::string headerField);

	static HttpRequest::Method	strToMethod(std::string methodStr);
};

#endif