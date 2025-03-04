/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:18:33 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/04 14:12:58 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include <sys/wait.h>
#include <fcntl.h>

CGIHandler::CGIHandler(const HttpRequest& request, HttpResponse& response, ServerSettings& server) : _request(request), _response(response), _server(server)
{
	_env = NULL;
	_cgiArgs = NULL;
}

CGIHandler::~CGIHandler()
{
	// int i = -1;
	// if (_env)
	// {
	// 	while (_env[++i] != NULL)
	// 		free(_env[i]);
	// 	delete[] _env;
	// }
	// if (_cgiArgs)
	// {
	// 	i = -1;
	// 	while (_cgiArgs[++i] != NULL)
	// 		free(_cgiArgs[i]);
	// 	delete[] _cgiArgs;
	// }
	delete[] _env;
	delete[] _cgiArgs;
}

void CGIHandler::setCGIPath()
{
	_cgiPath = _request.getTarget();
	
	size_t hasArgs = _cgiPath.find('?');
	if (hasArgs != std::string::npos)
	{
		_cgiEnv["QUERY_STRING"] = _cgiPath.substr(hasArgs + 1, _cgiPath.size());		
		_cgiPath = _cgiPath.substr(0, hasArgs);
	}
	
	struct stat buffer;
    if (stat(_cgiPath.c_str(), &buffer) != 0)
		throw std::runtime_error("CGI script path doesn't exist.");

	if (access(_cgiPath.c_str(), X_OK) != 0)
		throw std::runtime_error("CGI script path doesn't have execute permission.");
}

std::string CGIHandler::_getMethod(Method method)
{
	switch (method) {
        case GET:
			return "GET";
        case POST:
			return "POST";
        case DELETE:
			return "DELETE";
        default:
			return "UNKNOWN";
    }
}

void CGIHandler::setEnv()
{
	_cgiEnv["SERVER_NAME"] = _server.getServerName();
	_cgiEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	_cgiEnv["PATH_INFO"] = _cgiPath;
	_cgiEnv["REQUEST_METHOD"] = _getMethod(_request.getMethod());
	_cgiEnv["SCRIPT_FILENAME"] = _cgiPath;
	_cgiEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	_cgiEnv["CONTENT_TYPE"] = "text/plain"; //check this and content length from body size for post
}

void CGIHandler::_getCGIEnv()
{
	std::vector<std::string> envStrings;
    std::vector<char*> envPointers;

    for (std::map<std::string, std::string>::const_iterator it = _cgiEnv.begin(); it != _cgiEnv.end(); ++it) 
        envStrings.push_back(it->first + "=" + it->second);
		
    _env = new char*[envStrings.size() + 1];
    for (size_t i = 0; i < envStrings.size(); ++i)
	{
		_env[i] = new char[envStrings[i].size() + 1];
		
		for (size_t j = 0; j < envStrings[i].size(); ++j)
		_env[i][j] = envStrings[i][j];

		_env[i][envStrings[i].size()] = '\0';
	}
	
    _env[envStrings.size()] = NULL; 
}


void CGIHandler::CGIGet()
{
	_cgiArgs = new char*[3];
	std::string cgiExec = "/usr/bin/python3";
	_cgiArgs[0] = new char[cgiExec.size() + 1];
    for (size_t i = 0; i < cgiExec.size(); ++i)
        _cgiArgs[0][i] = cgiExec[i];
    _cgiArgs[0][cgiExec.size()] = '\0';

    _cgiArgs[1] = new char[_cgiPath.size() + 1];
    for (size_t i = 0; i < _cgiPath.size(); ++i)
        _cgiArgs[1][i] = _cgiPath[i];
    _cgiArgs[1][_cgiPath.size()] = '\0';
	_cgiArgs[2] = NULL; 
}

void CGIHandler::CGIPost()
{
	
}

void CGIHandler::getRequiredCGIArgs()
{
	switch (_request.getMethod())
	{
		case GET:
			CGIGet();
			break;
		case POST:
			CGIPost();
			break;
		default:
			break;
	}
}

void CGIHandler::execute()
{
	_getCGIEnv();
	// getRequiredCGIArgs();

	// std::cout << "QUERY" << _cgiEnv["QUERY_STRING"] << std::endl;
	int i = -1;
	while (_cgiArgs[++i] != NULL)
		std::cout << "ARG " << _cgiArgs[i] << std::endl;
	for (size_t i = 0; i < _cgiEnv.size(); i++)
		std::cout << "ENV: " << _env[i] << std::endl;
		
	const char* tempFileName = "/tmp/scriptresult";
	int fd = open(tempFileName, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd == -1)
		throw std::runtime_error("CGI file result creat failed.");

	int pipefd[2];
	if (pipe(pipefd) == -1)
		throw std::runtime_error("Pipe failed.");
	
	pid_t pid = fork();
	if (pid == -1)
		throw std::runtime_error("Fork failed.");
	
	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		
		std::cout << "CGI Execution!" << std::endl;
		execve(_cgiArgs[0], _cgiArgs, _env);
		std::cerr << "CGI Execution failed!" << std::endl;
        std::exit(1);
    } else { 
        close(fd);
		waitpid(pid, NULL, 0);
	}
	// _response.setStatusCode(200);
	_response.setBodyPath(tempFileName);
	
}

void CGIHandler::run()
{
	setCGIPath();
	setEnv();
	getRequiredCGIArgs();
	execute();	
}