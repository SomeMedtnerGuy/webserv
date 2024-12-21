/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:30:29 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/21 21:04:34 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

# include <string>
# include <vector>
# include <map>
# include <iostream>

class RequestParser
{
private:
    typedef std::map<std::string, std::string> headers_dict;
    enum Method {
        GET,
        POST,
        DELETE,
        UNKNOWN
    };
    std::vector<std::string> _methods;

    std::string _requestHeader;

    Method              _method;
    std::string         _target;
    headers_dict        _headers;
    int                 _statusCode;

    RequestParser();

    void    _preParser(void);
    void    _parseMethod(void);
    void    _parseTarget(void);
    void    _parseVersion(void);
    void    _parseHeaders(void);
    
public:
    RequestParser(std::string requestHeader);
    ~RequestParser();

    void    parse(void);
};

#endif