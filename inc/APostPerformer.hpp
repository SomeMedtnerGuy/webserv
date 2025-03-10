/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APostPerformer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:27:41 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 11:13:28 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_PERFORMER_HPP
# define POST_PERFORMER_HPP

# include "Socket.hpp" // for typedef
# include "StateMachine.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class APostPerformer
{
public:
    typedef Socket::data_container_t    data_t;
    
    virtual ~APostPerformer();

    virtual size_t  post(data_t& data) = 0;
    bool            isDone(void);

protected:
    APostPerformer(HttpResponse& response);
    HttpResponse&   _response;

    bool    _isDone;
        void    _setIsDone(bool newValue);
        bool    _getIsDone(void) const;
};

#endif