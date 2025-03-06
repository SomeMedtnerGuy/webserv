/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:30:21 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/06 11:21:35 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

# include "ConfigFile.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ServerSettings.hpp"
# include "PollManager.hpp"

// # include "RequestHandler.hpp"

# include <unistd.h>

class CGIHandler {
private:
	const HttpRequest& _request;
	HttpResponse& _response;
	std::string _cgiPath;
	// std::string _serverName;
	ServerSettings& _server;
	std::map<std::string, std::string> _cgiEnv;
	char **_env;
	char ** _cgiArgs;
	
	std::string _getMethod(Method method);
	void _getCgiEnv();
	void _redirectPipes(int fds[2]);

	
public:
	CGIHandler(const HttpRequest& request, HttpResponse& response, ServerSettings& server);
	// CGIHandler(const CGIHandler& src);
	// CGIHandler& operator=(const CGIHandler& src);
	~CGIHandler();

	bool isCgi(std::string& target);
	void CGIGet();
	void CGIPost();
	void getRequiredCgiArgs();
	void setCgiPath();
	void setEnv();
	void execute();
	void run();
};

#endif // CGIHANDLER_HPP