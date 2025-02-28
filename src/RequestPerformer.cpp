/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestPerformer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:09:54 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/28 13:48:05 by ndo-vale         ###   ########.fr       */
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
    std::cerr << "Performer called" << std::endl;
    
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
        //TODO
        _setIsDone(true);
    }
    /* THE FOLLOWING LINES ARE DEBUG */
    /*(void)data;
    _response.setBodyPath(_request.getTarget());
    _setIsDone(true);*/
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
	struct stat	info;
	if (stat(target.c_str(), &info) == -1) { // If stat fails, means the target does not exist
		_response.setStatusCode(404, _serverSettings.getErrorPage(404));
        return;
    }
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

void    RequestPerformer::_performDelete(void)
{
    //TODO
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