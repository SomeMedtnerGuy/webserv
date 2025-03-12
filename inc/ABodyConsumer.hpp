/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABodyConsumer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:27:41 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/12 09:42:03 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABODY_CONSUMER_HPP
# define ABODY_CONSUMER_HPP

# include "Socket.hpp" // for typedef
# include "StateMachine.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class ABodyConsumer
{
public:
    typedef Socket::data_container_t    data_t;
    
    virtual ~ABodyConsumer();

    virtual size_t  consume(data_t& data) = 0;
    bool            isDone(void);

protected:
    ABodyConsumer(HttpResponse& response, bool shouldPerformPost);
    HttpResponse&   _response;

    bool    _shouldPerformPost;
        void    _setShouldPerformPost(bool newValue);
        bool    _getShouldPerformPost(void) const;
    bool    _isDone;
        void    _setIsDone(bool newValue);
        bool    _getIsDone(void) const;
};

#endif