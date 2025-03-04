/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:30:21 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/04 09:25:30 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

# include "ConfigFile.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ServerSettings.hpp"
// # include "RequestHandler.hpp"

# include <unistd.h>

class CGIHandler {
private:
	const HttpRequest& _request;
	HttpResponse& _response;
	std::string _cgiPath;
	ServerSettings& _server;
	std::map<std::string, std::string> _cgiEnv;
	char **_env;
	char ** _cgiArgs;
	
	std::string _getMethod(Method method);
	void _getCGIEnv();
	
public:
	CGIHandler(const HttpRequest& request, HttpResponse& response, ServerSettings& server);
	// CGIHandler(const CGIHandler& src);
	// CGIHandler& operator=(const CGIHandler& src);
	~CGIHandler();

	void CGIGet();
	void CGIPost();
	void getRequiredCGIArgs();
	void setCGIPath();
	void setEnv();
	void execute();
	void run();
};

#endif // CGIHANDLER_HPP