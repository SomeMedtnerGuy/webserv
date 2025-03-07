/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:18:33 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/07 17:19:03 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include <sys/wait.h>
#include <fcntl.h>

CGIHandler::CGIHandler(const HttpRequest& request, HttpResponse& response, ServerSettings& server) : _request(request), _response(response), _server(server)
{
	_env = NULL;
	_cgiArgs = NULL;
	_isRunning = false;
}

CGIHandler::~CGIHandler()
{
	if (_env)
	{
		for (size_t i = 0; _env[i] != NULL; ++i)
			delete[] _env[i];
		delete[] _env;
	}

	if (_cgiArgs)
	{
		for (size_t i = 0; _cgiArgs[i] != NULL; ++i)
			delete[] _cgiArgs[i];
		delete[] _cgiArgs;
	}
}

bool CGIHandler::isCgi(std::string target)
{
	int pos = target.find("./root/cgi-bin");
	
	if (pos != 0/*  || target.size() > 15 */)
		return (false);
		
	return (true);
			//TODO change parser for cgi paths  
}

void CGIHandler::setCgiPath()
{
	_cgiPath = _request.getTarget();
	
	size_t hasArgs = _cgiPath.find('?');
	if (hasArgs != std::string::npos)
	{
		_cgiEnv["QUERY_STRING"] = _cgiPath.substr(hasArgs + 1, _cgiPath.size());		
		_cgiPath = _cgiPath.substr(0, hasArgs);
	}
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

void CGIHandler::_getCgiEnv()
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

void CGIHandler::getRequiredCgiArgs()
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
	_getCgiEnv();
	_isRunning = true;
	
	
	_tempFileName = "/tmp/scriptresult";
	int fd = open(_tempFileName, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd == -1)
		throw std::runtime_error("CGI file result creat failed.");
		
	_pid = fork();
	if (_pid == -1)
		throw std::runtime_error("Fork failed.");
	
	if (_pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		
		execve(_cgiArgs[0], _cgiArgs, _env);
		
		std::cerr << "CGI Execution failed!" << std::endl;
        std::exit(1);
    }
	
    // close(_pipefds[0]);
	// _pid = waitpid(pid, NULL, WNOHANG);
	// if (_pid == 0)
		
	// _response.setBodyPath(tempFileName);
	
}

bool CGIHandler::isCgiRunning(){return (_isRunning);}

bool CGIHandler::cgiDone()
{
	pid_t pidCheck = waitpid(_pid, NULL, WNOHANG);
	if (pidCheck != 0)
	{
		_response.setBodyPath(_tempFileName);
		return (true);
	}
	return (false);
}

void CGIHandler::_openPipe()
{
	if (pipe(_pipefds) == -1)
			throw std::runtime_error("Pipe failed.");
	PollManager::getInstance()->addDescriptor(_pipefds[1], POLLIN);
	
}

int CGIHandler::getReadPipe()
{
	return (_pipefds[1]);
}

void CGIHandler::run()
{
	
	setCgiPath();
	std::cerr << "\nSHIT\n" << std::endl;
	setEnv();
	getRequiredCgiArgs();
	execute();	
}