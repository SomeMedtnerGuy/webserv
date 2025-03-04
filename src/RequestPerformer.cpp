/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestPerformer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:09:54 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/04 16:09:52 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestPerformer.hpp"

RequestPerformer::RequestPerformer(HttpRequest& request, HttpResponse& response,
                                    ServerSettings& serverSettings)
    : AMessageHandler(request, response), _serverSettings(serverSettings),
        _consumeMode(false)
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
                dataConsumed = _performPost(data);
                break;
            default:
                throw (std::exception()); //TODO specify
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

//TODO use utils for the next few functions to check for validity of files
void    RequestPerformer::_performGet(void)
{
    std::string	target(_request.getTarget());
	struct stat	info;

	if (target.find("cgi-bin") != std::string::npos)
    {
        std::cout << "CGI!" << std::endl;
        CGIHandler cgi(_request, _response, _serverSettings);
        cgi.run();
		return ;
    }
	
	if (stat(target.c_str(), &info) == -1) { // If stat fails, means the target does not exist
		_response.setStatusCode(404, _serverSettings.getErrorPage(404));
        return;
    }
	if (info.st_mode & S_IFDIR) // If target is directory
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
		_response.setStatusCode(404, _serverSettings.getErrorPage(404));
		return ;
	}
	if (access(target.c_str(), W_OK) != 0) {
		_response.setStatusCode(403, _serverSettings.getErrorPage(403));
		return ;
	}
	std::remove(target.c_str());
	_response.setStatusCode(204, "");
}

size_t  RequestPerformer::_performPost(data_t data)
{
    (void)data;
    return (0);
    //TODO
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