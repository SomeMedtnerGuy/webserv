/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:23:44 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 11:34:04 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_PARSER_HPP
# define HTTP_PARSER_HPP

# define TOKEN_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&'*+-.^_`|~"

# include <string>

# include <HttpRequest.hpp>
# include <HttpResponse.hpp>

class HttpParser
{
public:
	static HttpResponse::status_code	httpSanitizer(std::string requestLine);
	static HttpResponse::status_code	parseRequestLine(HttpRequest& request, std::string requestLine);
	static HttpResponse::status_code	parseHeaderField(HttpRequest& request, std::string headerField);

	static HttpRequest::Method	strToMethod(std::string methodStr);
};

#endif