/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:42:09 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/25 16:21:53 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestProcessor.hpp"

RequestProcessor::RequestProcessor(ServerSettings& serverSettings, HttpRequest& request, HttpResponse& response)
	:_serverSettings(serverSettings), _request(request), _response(response) {}
RequestProcessor::~RequestProcessor(){}

void	RequestProcessor::process(void)
{
	_matchServer();
	_matchLocation();
	_checkReturnAndMethod();
	_buildFullTarget();
	_isThereABody();

	switch (_request.getMethod())
	{
	case GET:
		_performGet();
		break;
	case DELETE:
		_performDelete();
	}
	//Post will be performed while body is being read.
}

void	RequestProcessor::_matchServer(void)
{
	std::string	host;
	try {host = _request.getHeaders().at("Host");}
	catch (std::out_of_range& e) {throw RequestHandler::HttpError(400);}
	if (host.find(':') != host.length() - 5)
		// If the Host header has no port, since this server always listens on a custom port,
		// the request is badly built.
		throw RequestHandler::HttpError(400);
	host.erase(host.length() - 5); // Remove the port so host header becomes server name
	_serverSettings.setServer(host);
}

void	RequestProcessor::_matchLocation(void)
{
	_serverSettings.setLocation(_request.getTarget());
}

void	RequestProcessor::_checkReturnAndMethod(void)
{
	int	returnCode = _serverSettings.getReturnCode();
	if (returnCode != -1)
	{
		if (returnCode >= 300 && returnCode < 400)
			_response.addHeader("Location", _serverSettings.getReturnURL());
		throw RequestHandler::HttpError(_serverSettings.getReturnCode());
	}
	if (!_serverSettings.getAllowMethod(_request.getMethod()))
	{
		throw RequestHandler::HttpError(405);
	}
}

void	RequestProcessor::_buildFullTarget(void)
{
	std::cerr << "root: " << _serverSettings.getRoot() << std::endl;
	
	//TODO: Check this with Nicole
	std::string	target("root");
	target.append(_serverSettings.getRoot());
	
	target.append(_request.getTarget());
	_request.setTarget(target);
}

void	RequestProcessor::_isThereABody()
{
	const HttpMessage::headers_dict&	headers = _request.getHeaders();
	if (headers.find("Transfer-Encoding") != headers.end() 
		|| headers.find("Content-Lenght") != headers.end())
		_request.setHasBody(true);
}

void	RequestProcessor::_performGet(void)
{
	std::string	target(_request.getTarget());
	struct stat	info;
	if (stat(target.c_str(), &info) == -1) // If stat fails, means the target does not exist
		throw RequestHandler::HttpError(404);
	if (info.st_mode & S_IFDIR) // If target is directory
	{
		if (!_serverSettings.getAutoIndex())
		{
			target.append(_serverSettings.getIndex());
			_response.setBodyPath(target); //TODO: must be able to download maybe?
		}
		else
		{
			_createAutoIndex(target);
			_response.setBodyPath(".default/autoindex.html");
		}
	}
	else
		_response.setBodyPath(target);
}

void	RequestProcessor::_performDelete(void)
{
	std::string	target(_request.getTarget());
	struct stat	info;
	if (stat(target.c_str(), &info) == -1) // If stat fails, means the target does not exist
		throw RequestHandler::HttpError(404);
	if (access(target.c_str(), W_OK) != 0)
		throw RequestHandler::HttpError(403);
	std::remove(target.c_str());
	_response.setBodyPath(".default/default.html"); //TODO: Change to a more appropriate page
}

void	RequestProcessor::_createAutoIndex(std::string target)
{	
	DIR*	dir;
	struct dirent*	dp;
	if ((dir = opendir(target.c_str())) == NULL) {
        throw std::runtime_error("Could not open directory.");
    }
	std::ofstream	autoindexFile;
	autoindexFile.open(".default/autoindex.html");
	autoindexFile << 
		"<html>"
			"<head>"
				"<title>Index of " << target << "</title>"
			"</head>"
			"<body>"
				"<h1>Index of " << target << "</h1>"
				"<hr>"
				"<ul>";
	while ((dp = readdir(dir)) != NULL)
	{
		autoindexFile << "<li><a href=\"" << dp->d_name << "\">" << dp->d_name << "</a></li>";
	}
	autoindexFile <<
				"</ul>"
			"</body>"
		"</html>";
	closedir(dir);
	autoindexFile.close();
}

