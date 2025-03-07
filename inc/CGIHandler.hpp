/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:30:21 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/07 16:44:07 by nsouza-o         ###   ########.fr       */
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
	bool								_isRunning;
	pid_t 								_pid;
	const char* 						_tempFileName;
	// struct timeval 						_startTime;
	
	const HttpRequest& 					_request;
	HttpResponse&						_response;
	ServerSettings&						_server;
	std::map<std::string, std::string>	_cgiEnv;
	std::string							_cgiPath;
	char **								_env;
	char **								_cgiArgs;
	int									_pipefds[2];
	
	std::string _getMethod(Method method);
	void _getCgiEnv();
	void _openPipe();

	
public:
	CGIHandler(const HttpRequest& request, HttpResponse& response, ServerSettings& server);
	// CGIHandler(const CGIHandler& src);
	// CGIHandler& operator=(const CGIHandler& src);
	~CGIHandler();

	int getReadPipe();
	
	
	void CGIGet();
	void CGIPost();
	void getRequiredCgiArgs();
	void setCgiPath();
	void setEnv();
	void execute();
	void run();
	bool isCgiRunning();
	bool cgiDone();

	
	static bool isCgi(std::string target);
};

#endif // CGIHANDLER_HPP