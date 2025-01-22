/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:42:09 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/22 19:09:44 by ndo-vale         ###   ########.fr       */
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

	switch (_request.getMethod())
	{
	case GET:
		_performGet();
		break;
	case POST:
		_performPost();
		break;
	case DELETE:
		_performDelete();
		break; 
	default:
		throw RequestHandler::HttpError(501);
	}
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
	std::string	target(_serverSettings.getRoot());
	target.append(_request.getTarget());
	_request.setTarget(target);
}

void	RequestProcessor::_performGet(void)
{
	std::string	target(_request.getTarget());
	if (target.find_last_of('/') == target.length() - 1)
	{
		if (!_serverSettings.getAutoIndex())
			target.append(_serverSettings.getIndex());
		else
			; // TODO: Do some listing bullshit
	}
	//TODO: Should check with access() the permissions of the file, and whether the file exists or not
	_response.setBodyPath(target);
	std::cout << _response.getBodyPath() << std::endl;
}

void	RequestProcessor::_performPost(void)
{
	// TODO
}

void	RequestProcessor::_performDelete(void)
{
	// TODO
}