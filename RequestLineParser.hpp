/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:12:09 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/07 19:26:00 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_LINE_PARSER_HPP
# define REQUEST_LINE_PARSER_HPP

# include <string> //for std::string
# include <iostream> // for std::cout and std::endl
# include <cstring> // for std::memcpy

# define MAX_METHOD_SIZE 6

class RequestLineParser
{
    private:
        enum State 
		{
		    METHOD_PARSING,
		    TARGET_PARSING,
		    VERSION_PARSING,
		    FINISHED_PARSER
		};
        enum Method
        {
            INVALID,
            GET,
            POST,
            DELETE,
            METHODS_AM
        };
        
        static const std::string	_methods[3];

        State       _currentState;
        std::string _currentBuffer;
        
        Method      _method;
        std::string _target;

        void    _parseMethod(void);
        void    _parseTarget(void);
        void    _parseVersion(void);

    public:
        RequestLineParser();
        RequestLineParser(const RequestLineParser& other);
        RequestLineParser&  operator=(const RequestLineParser &other);
        ~RequestLineParser();
        
        void    updateAndParse(std::string& extraBuffer);
};

#endif