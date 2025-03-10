/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestPerformer.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:09:17 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 11:29:36 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PERFORMER
# define REQUEST_PERFORMER

# include <dirent.h>
# include <unistd.h>

# include "AMessageHandler.hpp"
# include "Socket.hpp"
# include "ServerSettings.hpp"
# include "APostPerformer.hpp"
# include "ChunkedPoster.hpp"
# include "RawPoster.hpp"

class RequestPerformer: public AMessageHandler
{
public:
    typedef Socket::data_container_t data_t;

    RequestPerformer(HttpRequest& request, HttpResponse& response,
                        ServerSettings& serverSettings);
    ~RequestPerformer();

    size_t  perform(const data_t& data);
    void    activateConsumeMode(void);
private:
    ServerSettings& _serverSettings;
    bool    _consumeMode;
        void    _setConsumeMode(bool newValue);
        bool    _getConsumeMode(void) const;
    
    APostPerformer*  _postPerformer;

    void    _performGet(void);
    void    _performDelete(void);
    size_t  _performPost(data_t data);

    void    _createAutoIndex(std::string target);
};

#endif