/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 12:20:15 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/02 18:03:22 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# define DEFAULT_PAGE_PATH "./.default/default.html"
# define HTTP_VERSION "HTTP/1.1"

# include "HttpMessage.hpp"
# include "Helpers.hpp"

class HttpResponse: public HttpMessage
{
private:
    int         _statusCode;
    std::string _bodyPath;

    HttpResponse(const HttpResponse& other);
    HttpResponse&   operator=(const HttpResponse& other);

public:
    void    setStatusCode(int statusCode);
    void    setBodyPath(std::string bodyPath);
    
    int         getStatusCode(void) const;
    std::string getBodyPath(void) const;

    HttpResponse();
    ~HttpResponse();
};

#endif