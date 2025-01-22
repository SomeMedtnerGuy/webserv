/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:35:16 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/22 16:27:28 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PROCESSOR_HPP
# define REQUEST_PROCESSOR_HPP

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
		void	_performGet(void);
		void	_performPost(void);
		void	_performDelete(void);
	public:
		RequestProcessor(ServerSettings& handler, HttpRequest& request, HttpResponse& response);
		~RequestProcessor();
		void	process(void);
    };

#endif