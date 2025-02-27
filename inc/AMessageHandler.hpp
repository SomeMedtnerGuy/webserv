/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:44:49 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 20:05:59 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_MESSAGE_HANDLER
# define A_MESSAGE_HANDLER

# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class AMessageHandler
{
public:
    AMessageHandler(HttpRequest& request, HttpResponse& response);
    ~AMessageHandler();
    
    virtual void    handle(void) = 0;
    bool            isDone(void) const;

protected:
    HttpRequest&    _request;
    HttpResponse&   _response;

    bool    _isDone;
        void    _setIsDone(bool value);
        bool    _getIsDone(void) const;
};

#endif