/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:11:41 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/27 14:21:03 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RequestHandler.hpp"



/* PUBLIC */
RequestHandler::RequestHandler(int sockfd, ConfigFile& configs)
	: _sockfd(sockfd),  _flags(0), _serverSettings(configs), _stash(0)
{
    std::memset(_buffer, '\0', BUFFER_SIZE + 1);
	
}
RequestHandler::~RequestHandler() {}

void    RequestHandler::handleRequest(void)
{
	try {
		if (!_isFlagOn(HEADER_PROCESSED)) {
			std::string	requestHeader = _receiveRequestHeader();
			std::cerr << "REQUEST HEADER: " << std::endl << requestHeader << std::endl;
			RequestParser		requestParser(requestHeader, _requestObj);
			requestParser.parse();
			RequestProcessor	requestProcessor(_serverSettings, _requestObj, _responseObj);
			requestProcessor.process();
			_setFlags(HEADER_PROCESSED);
		}
	} catch (const HttpError& e) {
		std::cerr << e.getStatusCode() << std::endl;
		_responseObj.setStatusCode(e.getStatusCode());
		_setFlags(REQUEST_PROCESSED);
		switch (_responseObj.getStatusCode()) {
			case 400: case 408: case 413: case 414: case 431: case 500: case 503: //Codes which should terminate connection immediately
				_setFlags(CLOSE_CONNECTION);
				_setFlags(REQUEST_PROCESSED);
				return ;
		}
	}
	try {
		if (!_isFlagOn(REQUEST_PROCESSED))
		{
			switch (_requestObj.getMethod())
			{
				case GET:
					if (!_isFlagOn(METHOD_PERFORMED))
						_performGet();
					_readSomeBody();
					break;
				case DELETE:
					if (!_isFlagOn(METHOD_PERFORMED))
						_performDelete();
					_readSomeBody();
					break;
				case POST:
					_readSomeBody();
					_performPost();
					break;
				default:
					;
			}
			// TODO: receive and process body depending on method
			_setFlags(REQUEST_PROCESSED);
		}
	} catch (const HttpError& e) { //TODO: Probably should create function out of this
		std::cerr << e.getStatusCode() << std::endl;
		_responseObj.setStatusCode(e.getStatusCode());
		_setFlags(REQUEST_PROCESSED);
		switch (_responseObj.getStatusCode()) {
			case 400: case 408: case 413: case 414: case 431: case 500: case 503: //Codes which should terminate connection immediately
				_setFlags(CLOSE_CONNECTION);
				_setFlags(REQUEST_PROCESSED);
				return ;
		}
	}

	if (_isFlagOn(REQUEST_PROCESSED)) {
		// TODO: Send response. MUST CHECK HOW TO SEND BODY IN PIECES
		
		_sendResponse(); //TODO: EXPERIMENTAL ONLY

		
		_setFlags(REQUEST_HANDLED);
	}
	


	/* ORDER OF OPERATIONS */
	
	// IN ONE GO
    	// 1. Receive Request Header Section
		// 2. Parse Request Header Section
		// 3. Use parsed info to process the request
	// Things to consider:
		// 1. Rest of buffer might have body. Must save it somewhere
		// 2. Any of the above might throw an error. Must be in a try-catch
	
	// IN PIECES (if there is body):
		// 1. Receive body from sockfd
		// IMPORTANT: Body reading is dependant of the processing!! (method, etc)
	// Things to consider:
		// 1. If first time, should get the beginning of the body from before
		// Perhaps should be saved in a file directly on the previous step??
		// 2. Since body might be really big, maybe might be necessary to save it in a file
		// 3. Body reader has to have access to some info from before, like body size, encoding type, filename, etc.
	
	// AS SOON AS ABOVE IS DONE
		// 1. Send response response line and headers
	// IN PIECES (if there is body):
		// 2. Write body in sockfd
	// Things to consider:
		// 1. Response must be built beforehand. This section must have access to it
}

bool	RequestHandler::isRequestHandled(void)
{ return (_isFlagOn(REQUEST_HANDLED)); }
bool	RequestHandler::shouldCloseConnection(void)
{ return (_isFlagOn(CLOSE_CONNECTION)); }

/* EXCEPTIONS */
RequestHandler::RecvSendError::RecvSendError(){}
RequestHandler::RecvSendError::RecvSendError(int errorCode)
	: _errorCode(errorCode) {}
RequestHandler::RecvSendError::~RecvSendError() throw() {}
int	RequestHandler::RecvSendError::getErrorCode(void) const throw()
{ return (_errorCode);}

RequestHandler::HttpError::HttpError(){}
RequestHandler::HttpError::HttpError(int statusCode)
	: _statusCode(statusCode) {}
RequestHandler::HttpError::~HttpError() throw() {}
int	RequestHandler::HttpError::getStatusCode(void) const throw()
{ return (_statusCode);}


/* PRIVATE */
void    RequestHandler::_setFlags(int flags) {_flags |= flags;}
void    RequestHandler::_unsetFlags(int flags) {_flags &= ~flags;}
int     RequestHandler::_getFlags(void) const {return (_flags);}
bool    RequestHandler::_isFlagOn(int flag) const {return (_flags & flag);}

std::string    RequestHandler::_receiveRequestHeader(void)
{
    std::string requestHeader;
    std::size_t	endOfHeader = requestHeader.npos;

    while (requestHeader.length() < MAX_HEADER_SIZE
        && endOfHeader == requestHeader.npos)
    {
        std::memset(_buffer, '\0', BUFFER_SIZE);
        int recvStatus = recv(_sockfd, _buffer, BUFFER_SIZE, 0);
        if (recvStatus <= 0)
            throw (RecvSendError(recvStatus));	// If a request is incomplete (there is no \r\n\r\n), this will be thrown indefinitely
												// The timeout should solve this
		requestHeader.append((char*)_buffer);
		endOfHeader = requestHeader.find("\r\n\r\n");
    }
    if (requestHeader.find("\r\n") > BUFFER_SIZE) {
        throw (HttpError(414));}
    else if (endOfHeader == requestHeader.npos) {
        throw (HttpError(431));}
	_stash.assign(_buffer + endOfHeader + 4, _buffer + BUFFER_SIZE + 1); //Save the rest of the buffer in the stash
    requestHeader.erase(endOfHeader + 4);
    return (requestHeader);
}

void	RequestHandler::_readSomeBody(void)
{
	
}

void	RequestHandler::_performGet(void)
{
	std::string	target(_requestObj.getTarget());
	struct stat	info;
	if (stat(target.c_str(), &info) == -1) // If stat fails, means the target does not exist
		throw RequestHandler::HttpError(404);
	if (info.st_mode & S_IFDIR) // If target is directory
	{
		if (!_serverSettings.getAutoIndex())
		{
			target.append(_serverSettings.getIndex());
			_responseObj.setBodyPath(target); //TODO: must be able to download maybe?
		}
		else
		{
			_createAutoIndex(target);
			_responseObj.setBodyPath(".default/autoindex.html");
		}
	}
	else
		_responseObj.setBodyPath(target);
}

void	RequestHandler::_performDelete(void)
{
	std::string	target(_requestObj.getTarget());
	struct stat	info;
	if (stat(target.c_str(), &info) == -1) // If stat fails, means the target does not exist
		throw RequestHandler::HttpError(404);
	if (access(target.c_str(), W_OK) != 0)
		throw RequestHandler::HttpError(403);
	std::remove(target.c_str());
	_responseObj.setBodyPath(".default/default.html"); //TODO: Change to a more appropriate page
}

void	RequestHandler::_performPost(void)
{
	
}

void	RequestHandler::_createAutoIndex(std::string target)
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


/* THIS FUNCTION IS BEING USED JUST FOR TESTING, MUST BE REVIEWED */
void	RequestHandler::_sendResponse(void)
{
	std::stringstream response;
    std::ifstream file(_responseObj.getBodyPath().c_str(), std::ios::binary);
    std::string fileLine;
    std::string sendBuff;
    
    response << HTTP_VERSION << " " << _responseObj.getStatusCode() << "\r\n";
    const HttpMessage::headers_dict headers = _responseObj.getHeaders();
    HttpMessage::headers_dict::const_iterator cit;
    for (cit = headers.begin(); cit != headers.end(); cit++)
        response << cit->first << ": " << cit->second << "\r\n";
    response << "\r\n";
	response << file.rdbuf();
    file.close();
    sendBuff = response.str();
	std::cerr << "RESPONSE: " << std::endl << sendBuff << std::endl; 
    send(_sockfd, sendBuff.c_str(), sendBuff.length(), 0);
}