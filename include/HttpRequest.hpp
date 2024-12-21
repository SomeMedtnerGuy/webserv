/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:48:16 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/21 21:06:11 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string> // For string
# include <map> // For map
#include <sys/socket.h> // For recv

# define BUFFERSIZE 24
# define MAX_REQUEST_HEADER_SIZE 32000


class HttpRequest
{
private:
    
    
    //FLAGS
    bool     _eof;
    bool    _parserDone;

    int         _sockfd;
    char        _buffer[BUFFERSIZE];
    ssize_t     _recvStatus;
    std::string _buffRemainder;

    HttpRequest();

    void    _parseRequest(void);
    
public:
    HttpRequest(int sockfd);
    ~HttpRequest();

    bool    eof(void) const;
    ssize_t recvStatus(void) const;

    int    handleRequest(void);
};

#endif
