/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:42:09 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/07 16:30:19 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestProcessor.hpp"

RequestProcessor::RequestProcessor(ServerSettings& serverSettings, HttpRequest& request, HttpResponse& response)
	: _serverSettings(serverSettings), _request(request), _response(response) {}
RequestProcessor::~RequestProcessor(){}

void	RequestProcessor::process(void)
{
	_matchServer();
	_matchLocation();
	_checkReturnAndMethod();
	_buildFullTarget();
	_isThereABody();
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
	std::string	target(""); // std::string	target("root");
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



