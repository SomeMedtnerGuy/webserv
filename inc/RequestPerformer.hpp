/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestPerformer.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:09:17 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 21:43:34 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PERFORMER
# define REQUEST_PERFORMER

# include "AMessageHandler.hpp"
# include "Socket.hpp"

class RequestPerformer: public AMessageHandler
{
public:
    RequestPerformer(HttpRequest& request, HttpResponse& response);
    ~RequestPerformer();

    size_t  perform(Socket::data_container_t data);
    void    activateConsumeMode(void);
private:
    bool    _consumeMode;
        void    _setConsumeMode(bool newValue);
        bool    _getConsumeMode(void) const;
};

#endif