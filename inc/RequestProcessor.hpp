/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:35:16 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/27 14:04:06 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PROCESSOR_HPP
# define REQUEST_PROCESSOR_HPP

# include <dirent.h> // To open directories
# include <unistd.h> // For access()

# include "ServerSettings.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "RequestHandler.hpp" //For exceptions only

class	RequestProcessor
{
private:
		ServerSettings&	_serverSettings;
		HttpRequest&	_request;
		HttpResponse&	_response;
		
		void	_matchServer(void);
		void	_matchLocation(void);
		void	_buildFullTarget(void);
		void	_checkReturnAndMethod(void);
		void	_isThereABody(void);
		void	_performGet(void); //TODO: Either i keep these two here or in the object that will process body (Because POST will be there too) (PROBABLY BETTER THE LATTER)
		void	_performDelete(void);

		void	_createAutoIndex(std::string target);
	public:
		RequestProcessor(ServerSettings& handler, HttpRequest& request, HttpResponse& response);
		~RequestProcessor();
		void	process(void);
    };

#endif