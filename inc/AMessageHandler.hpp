/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:44:49 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/28 11:21:47 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_MESSAGE_HANDLER
# define A_MESSAGE_HANDLER

# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class AMessageHandler
{
public:
    ~AMessageHandler();
    bool    isDone(void) const;

protected:
    AMessageHandler(HttpRequest& request, HttpResponse& response);
    
    HttpRequest&    _request;
    HttpResponse&   _response;

    bool    _isDone;
        void    _setIsDone(bool value);
        bool    _getIsDone(void) const;
};

#endif