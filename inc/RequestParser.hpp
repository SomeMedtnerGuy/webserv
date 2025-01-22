/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:22 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/03 08:38:47 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

# define VERSION "HTTP/1.1\r\n"
# define TOKEN_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&'*+-.^_`|~"

# include "HttpRequest.hpp"
# include "RequestHandler.hpp"
# include <string>
# include <vector>

class RequestParser
{
private:
    std::string     _requestHeader;
    HttpRequest&     _request;

    RequestParser(const RequestParser& other);

    void        _preParser(void);
    void        _parseMethod(void);
    void        _parseTarget(void);
    void        _parseVersion(void);
    void        _parseHeaders(void);
    std::string _parseFieldName(std::string& field);
    std::string _parseFieldValue(std::string& field);
    
public:
    RequestParser(std::string requestHeader, HttpRequest& request);
    ~RequestParser();

    void    parse(void);
};

#endif