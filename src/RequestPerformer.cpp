/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestPerformer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:09:54 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/08 17:17:24 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestPerformer.hpp"

RequestPerformer::RequestPerformer(HttpRequest& request, HttpResponse& response,
                                    ServerSettings& serverSettings)
    : AMessageHandler(request, response), _serverSettings(serverSettings),
        _consumeMode(false), _postPerformer(NULL)
{}
RequestPerformer::~RequestPerformer(){}

size_t    RequestPerformer::perform(const data_t& data)
{
    size_t  dataConsumed = 0;
    if (_getConsumeMode() == false) {
        switch (_request.getMethod()) {
            case GET:
                _performGet();
                _setConsumeMode(true);
                break;
            case DELETE:
                _performDelete();
                _setConsumeMode(true);
                break;
            case POST:
				// std::cout << "perform post" << std::endl;
                dataConsumed = _performPost(data);
                break;
            default:
                throw (std::runtime_error("Request manager achieved an unexpected state."));
        };
    }
    if (_getConsumeMode() == true) {
		if (_request.getBodySize() != -1) { //If theres body to consume
			size_t	bodySize = static_cast<size_t>(_request.getBodySize());
			// I want to consume as much as possible. That means as much as bodysize there is in data
        	dataConsumed += std::min(bodySize, data.size());
			if (dataConsumed == bodySize) {
				_setIsDone(true);
			}
		} else { // If there is not, just fuck off
			_setIsDone(true);
		} 
    }
    return (dataConsumed);
}

void    RequestPerformer::activateConsumeMode(void)
{
    _setConsumeMode(true);
}

void    RequestPerformer::_setConsumeMode(bool newValue) {_consumeMode = newValue;}
bool    RequestPerformer::_getConsumeMode(void) const {return (_consumeMode);}

void    RequestPerformer::_performGet(void)
{
    std::string	target(_request.getTarget());
	if (!(isFile(target) || isDirectory(target))) { // If stat fails, means the target does not exist
		_response.setStatusCode(404);
        return;
    }
	if (isDirectory(target)) // If target is directory
	{
		if (!_serverSettings.getAutoIndex())
		{
			target.append(_serverSettings.getIndex());
			_response.setBodyPath(target);
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

void    RequestPerformer::_performDelete(void)
{
	std::string	target(_request.getTarget());
    if (!isFile(target)) {
		_response.setStatusCode(404);
		return ;
	}
	if (access(target.c_str(), W_OK) != 0) {
		_response.setStatusCode(403);
		return ;
	}
	std::remove(target.c_str());
	_response.setStatusCode(204);
}

size_t  RequestPerformer::_performPost(data_t data)
{
	size_t	dataConsumed = 0;
	if (!_postPerformer) {
		const HttpMessage::headers_dict	requestHeaders = _request.getHeaders();
		if (requestHeaders.find("Transfer-Encoding") != requestHeaders.end()) {
			_postPerformer = new ChunkedPoster(_response, _request.getTarget());
		} else if (requestHeaders.find("Content-Length") != requestHeaders.end()) {
			_postPerformer = new RawPoster(_response, _request.getTarget(), _request.getBodySize());
		} else { // It should never get to this point, because the presence of a body was already previously checked
			throw (std::runtime_error("Body is erroneously expected"));
		}
	}
	if (_response.getStatusCode() != 200) {
		return (dataConsumed);
	}
	dataConsumed += _postPerformer->post(data);
	if (_postPerformer->isDone()) {
		delete _postPerformer;
		_postPerformer = NULL;
		_setIsDone(true);
	}
    return (dataConsumed);
}

void	RequestPerformer::_createAutoIndex(std::string target)
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