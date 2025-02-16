/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:06:46 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 22:56:09 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

/* PUBLIC */
RequestManager::RequestManager(Socket& socket, ConfigFile& configFile)
	: _stateMachine(STATE_SIZE), _socket(socket), _serverSettings(configFile, getPortFromSocket(_socket.getSockfd())) {}
RequestManager::~RequestManager() {}

void	RequestManager::handleMoreRequest(void)
{
	unsigned int	currentState = _stateMachine.getCurrentState();
	unsigned int	previousState = STATE_SIZE; //Any invalid state works
	while (currentState != previousState) {
		switch (_stateMachine.getCurrentState()) {
			case PARSING_REQUEST_LINE:
				//std::cerr << "PARSING_REQUEST_LINE" << std::endl; //TODO Put this back to printing as soon as this is only called by connection when there is something to write
				_parseRequestLine();
				break;
			case PARSING_HEADERS:
				std::cerr << "PARSING_HEADERS" << std::endl;
				_parseHeaders();
				break;
			case PROCESSING_REQUEST:
				std::cerr << "PROCESSING_REQUEST" << std::endl;
				_processRequest();
				break;
			case PERFORMING_REQUEST:
				std::cerr << "PERFORMING_REQUEST" << std::endl;
				_performRequest();
				break;
			case RESPONDING_HEADER:
				std::cerr << "RESPONDING_HEADER" << std::endl;
				_sendResponseHeader();
				break;
			case RESPONDING_BODY:
				std::cerr << "RESPONDING_BODY" << std::endl;
				_sendResponseBody();
				break;
			case DONE:
				std::cerr << "DONE" << std::endl;
				_setHandlingComplete(true);
				break;
			default:
				//TODO: Remove before production
				std::cerr << "Some unexpected state happened at RequestManager. Aborting" << std::endl;
				std::cerr << "Current state: " << _stateMachine.getCurrentState() << std::endl;
				std::abort(); 
		}
		previousState = currentState;
		currentState = _stateMachine.getCurrentState();
	}
	_socket.resetState();
}

bool	RequestManager::isHandlingComplete(void) const {return (_getHandlingComplete());}
bool	RequestManager::shouldCloseConnection(void) const {return (_getCloseConnection());}

/* PRIVATE */
void	RequestManager::_setCloseConnection(bool value) {_closeConnection = value;}
bool	RequestManager::_getCloseConnection(void) const {return (_closeConnection);}
void	RequestManager::_setHandlingComplete(bool value) {_handlingComplete = value;}
bool	RequestManager::_getHandlingComplete(void) const {return (_handlingComplete);}

//This function must receives a request and updates the objects accordingly.
void	RequestManager::_parseRequestLine(void)
{
	//Do the following
	do {
		// check the stash for a full request line
		Socket::data_container stash = _socket.getStash();
		std::string	requestLine(stash.begin(), stash.end());
		size_t	requestLineSize = requestLine.find("\r\n");
		// if it is found
		if (requestLineSize != requestLine.npos && requestLineSize <= BUFFER_SIZE)
		{
			//process it, consume it, advance state and return
			code parsingResult = _parseRequestLine(requestLine.substr(0, requestLineSize));
			if (parsingResult != 200)
				_abortRequestHandling(parsingResult);
			else {
				_socket.consumeFromStash(requestLineSize + 2);
				_stateMachine.advanceState();
			}
			return;
		}
		// else if the request line becomes bigger than it should
		else if (requestLineSize > BUFFER_SIZE && requestLineSize != requestLine.npos)
		{
			//generate 414 response, indicate that the connection should be closed and jump to answering state
			_abortRequestHandling(414);
			return ;
		}
		//If it got here, none apply, so more socket must be read.
	}
	//while you can read from the socket
	while (_socket.readToStash() > 0);
	//If here, it means you cannot read from the socket anymore, so just quietly return
	return ;
}

void	RequestManager::_parseHeaders(void)
{
	static size_t	headers_size = 0;
	Socket::data_container stash = _socket.getStash();
	std::string	stashStr(stash.begin(), stash.end());
	//While the header section hasn't finished
	while (stashStr.compare(0, 2, "\r\n") != 0)
    {
        size_t      fieldEnd = stashStr.find("\r\n");
		//If you cant find a complete header field, update stash and repeat
		if (fieldEnd == stashStr.npos)
		{
			ssize_t readResult = _socket.readToStash();
			if (readResult <= 0) //You cannot read anymore!
				return;
			else
			{
				stash = _socket.getStash();
				stashStr = std::string(stash.begin(), stash.end());
				continue;
			}
		}
		
        std::string field = stashStr.substr(0, fieldEnd);
		code	parsingResult = _parseHeaderField(field);
		if (parsingResult != 200)
		{
			_abortRequestHandling(parsingResult);
			return ;
		}
		_socket.consumeFromStash(fieldEnd + 2);
        stashStr.erase(0, fieldEnd + 2);
		headers_size += fieldEnd + 2;
		if (headers_size > BUFFER_SIZE * 4)
		{
			_abortRequestHandling(parsingResult);
			return;
		}
    }
	_socket.consumeFromStash(2); //Take the "\r\n" away
	_stateMachine.advanceState();
}

void	RequestManager::_processRequest(void)
{
	//Check return directive
	int	returnCode = _serverSettings.getReturnCode();
	if (returnCode != -1)
	{
		if (returnCode >= 300 && returnCode < 400)
			_response.addHeader("Location", _serverSettings.getReturnURL());
		//This next line is important, because if it is a GET request, there might be a body 
		//that will get in the middle of the next request if the connection stays open
		_setCloseConnection(true); 
		_abortRequestHandling(returnCode);
		return ;
	}
	//Check allowed method
	if (!_serverSettings.getAllowMethod(_request.getMethod()))
	{
		_setCloseConnection(true); //Same reasoning as before
		_abortRequestHandling(405);
	}
	//build full target
	std::string	target(_serverSettings.getRoot());
	target.append(_request.getTarget());
	_request.setTarget(target);
	//Additional checks
	const HttpMessage::headers_dict&	headers = _request.getHeaders();
	if (_request.getMethod() == POST
		&& headers.find("Content-Length") == headers.end()
		&& headers.find("Transfer-Encoding") == headers.end())
	{
		_abortRequestHandling(400);
	}
	
	if (headers.find("Content-Lenght") != headers.end())
	{
		std::string	sizeStr = headers.at("Content-Length");
		ssize_t bodySize = std::atoi(sizeStr.c_str()); //TODO Should check iff it is an integer
		if (bodySize > static_cast<ssize_t>(_serverSettings.getClientBodySize()))
			_abortRequestHandling(413);
		_request.setBodySize(std::atoi(sizeStr.c_str()));
	}
	_stateMachine.advanceState();
}

void	RequestManager::_performRequest(void)
{
	//TODO complete
	if (_request.getMethod() == GET)
	{
		_performGet();
	}
	/*if (_request.getBodySize() != -1)
	{
		if (_request.getMethod() == POST)
		{
		//	static stream 
		}
	}*/
		
	_stateMachine.advanceState();
}

void	RequestManager::_sendResponseHeader(void)
{
	std::stringstream response;    
    response << "HTTP/1.1 " << _response.getStatusCode() << "\r\n";
    const HttpMessage::headers_dict headers = _response.getHeaders();
    HttpMessage::headers_dict::const_iterator cit;
    for (cit = headers.begin(); cit != headers.end(); cit++)
        response << cit->first << ": " << cit->second << "\r\n";
    response << "\r\n";
    std::string	sendBuff = response.str(); 
    _socket.sendToSocket(reinterpret_cast<const unsigned char*>(sendBuff.c_str()), sendBuff.length());
	_stateMachine.advanceState();
}

void	RequestManager::_sendResponseBody(void)
{
	//TODO must complete
	std::stringstream response;
	std::ifstream file(_response.getBodyPath().c_str(), std::ios::binary);
	response << file.rdbuf();
    file.close();
	response << "\r\n";
	std::string	sendBuff = response.str(); 
    ssize_t	sendToSocketResult = _socket.sendToSocket(reinterpret_cast<const unsigned char*>(sendBuff.c_str()), sendBuff.length());
	if (sendToSocketResult == -1)
		return;
	_stateMachine.advanceState();
}

void	RequestManager::_abortRequestHandling(HttpResponse::status_code statusCode)
{
	std::cerr << "ABORT! error code: " << statusCode << std::endl;
	_response.setStatusCode(statusCode, _serverSettings.getErrorPage(statusCode));
	if (statusCode == 404) //TODO put the rest of the close connection codes
		_setCloseConnection(true);
	_stateMachine.setCurrentState(RESPONDING_HEADER);
}

HttpResponse::status_code	RequestManager::_httpSanitizer(std::string requestLine)
{
	size_t  crPos = requestLine.find('\r');;
    do
    {
        if (crPos != requestLine.npos && requestLine[crPos + 1] != '\n')
            return (400);
        crPos++;
        crPos = requestLine.find('\r', crPos);
    } while (crPos != requestLine.npos);
	return (200);
}
HttpResponse::status_code	RequestManager::_parseRequestLine(std::string requestLine)
{
	//Initial sanitizer
	HttpResponse::status_code	sanitizerResult = _httpSanitizer(requestLine);
	if (sanitizerResult != 200)
		{return (sanitizerResult);}
	if (requestLine.compare(0, 2, "\r\n") == 0)
        requestLine.erase(0, 2);

	//Set Method
    _request.setMethod(_strToMethod(requestLine));
    requestLine.erase(0, requestLine.find(" ") + 1);
    if (_request.getMethod() == UNKNOWN)
        {return (501);}
	
	//Set target
	std::size_t separatorPos = requestLine.find(' ');
	
    _request.setTarget(requestLine.substr(0, separatorPos));
    requestLine.erase(0, separatorPos + 1);

	//Make sure the version is correct
	std::string	version = "HTTP/1.1";
	if (requestLine.compare(version) != 0)
        {return (400);}
	
	//All is good
	return (200);
}

HttpResponse::status_code	RequestManager::_parseHeaderField(std::string headerField)
{
	std::string	fieldName;
	std::string	fieldValue;
	
	//Parse fieldName	
	const size_t  colonPos = headerField.find(':');
    if (colonPos == headerField.npos)
        {return (400);}
    fieldName = headerField.substr(0, colonPos);
    if (fieldName.find_first_not_of(TOKEN_CHARS) != fieldName.npos)
        {return (400);}
	
	//Parse fieldValue
	fieldValue = headerField.substr(colonPos + 1);
    std::string::iterator   it = fieldValue.begin();
    while (*it == ' ' || *it == '\t')
        it++;
    fieldValue.erase(fieldValue.begin(), it);
    std::string::reverse_iterator   rit = fieldValue.rbegin();
    while (*rit == ' ' || *rit == '\t')
        rit++;
    fieldValue.resize(fieldValue.length() - (rit - fieldValue.rbegin()));
	//Additional checking can be done here:
	if (fieldName.compare("Host") == 0)
	{
		std::size_t	colonPos = fieldValue.find(':');
		_serverSettings.setServer(std::string(fieldValue, 0, colonPos));
		_serverSettings.setLocation(_request.getTarget());
		if (_serverSettings.getAllowMethod(_request.getMethod()) == false) //TODO: Cant put this shit here. Must read everything in body just in case. Move it to processing
		{
			return (405);
		}
	}
	//Place the header in the request.
	_request.addHeader(fieldName, fieldValue);
    return (200);
}

void	RequestManager::_performGet(void)
{
	std::string	target(_request.getTarget());
	struct stat	info;
	if (stat(target.c_str(), &info) == -1) // If stat fails, means the target does not exist
		_abortRequestHandling(404);
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

void	RequestManager::_createAutoIndex(std::string target)
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

void	RequestManager::_performDelete(void)
{
	//TODO
}
void	RequestManager::_performPost(void)
{
	//TODO
}

Method  RequestManager::_strToMethod(std::string str)
{
    const std::string methods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++)
	{
		if (str.compare(0, methods[i].length() + 1, methods[i] + " ") == 0)
			return (static_cast<Method>(i));
	}
	return (UNKNOWN);
}