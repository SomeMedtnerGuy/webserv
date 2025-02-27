/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OldRequestHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:03:27 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 17:04:32 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* #include "OldRequestHandler.hpp"

RequestHandler::RequestHandler(Socket& socket, ConfigFile& configFile)
    : _stateMachine(STATE_SIZE, PARSING_REQUEST_LINE),
        _socket(socket), _serverSettings(configFile, getPortFromSocket(socket.getSockFd())) {};
RequestHandler::~RequestHandler() {};

void    RequestHandler::handle(void)
{
    State   currentState = _stateMachine.getCurrentState();
    State   previousState = currentState;
    do {
        switch (currentState) {
            case PARSING_REQUEST_LINE:
                std::cerr << "parsing request line" << std::endl;
               _parseRequestLine();
               break;
            case PARSING_HEADERS:
                std::cerr << "parsing headers" << std::endl;
                _parseHeaders();
                break;
            case PROCESSING_REQUEST:
                std::cerr << "processing request" << std::endl;
                _processRequest();
                break;
            case CONSUME_BODY:
                std::cerr << "consume body" << std::endl;
                _consumeBody();
                break;
            case RESPONDING_HEADER:
                std::cerr << "responding header" << std::endl;
                _sendResponseHeader();
                break;
            case RESPONDING_BODY:
                std::cerr << "responding body" << std::endl;
                _sendResponseBody();
                break;
            case DONE:
                std::cerr << "done" << std::endl;
                _setHandlingComplete(true);
                break;
            default:
                std::cerr << "Some unexpected state happened!" << std::endl;
                throw (std::exception());
        }
        previousState = currentState;
        currentState = _stateMachine.getCurrentState();
            //std::cerr << previousState << "  " << currentState << std::endl;
    } while (currentState != previousState);
}

bool    RequestHandler::isReceiving(void) const
{
    State   currentState = _stateMachine.getCurrentState();
    switch (currentState) {
        case PARSING_REQUEST_LINE:
        case PARSING_HEADERS:
        case PROCESSING_REQUEST:
        case CONSUME_BODY:
            return (true);
            break;
        default:
            return (false);
    }
}

bool    RequestHandler::isSending(void) const
{
    State   currentState = _stateMachine.getCurrentState();
    switch (currentState) {
        case RESPONDING_HEADER:
        case RESPONDING_BODY:
            return (true);
        default:
            return (false);
    }
}

bool    RequestHandler::isDone(void) const {return (_getHandlingComplete());}
bool    RequestHandler::shouldCloseConnection(void) const {return (_getCloseConnection());}

void    RequestHandler::_setHandlingComplete(bool value) {_handlingComplete = value;}
bool    RequestHandler::_getHandlingComplete(void) const {return (_handlingComplete);}
void    RequestHandler::_setCloseConnection(bool value) {_closeConnection = value;}
bool    RequestHandler::_getCloseConnection(void) const {return (_closeConnection);}

void	RequestHandler::_parseRequestLine(void)
{
    const Socket::data_container_t& stash = _socket.getRecvStash();
    std::string                     stashStr(stash.begin(), stash.end());
    size_t                          requestLineSize = stashStr.find(DELIMITOR);

    if (requestLineSize != stashStr.npos) { //found
        if (requestLineSize > BUFFER_SIZE) { //line too big
            _abortRequestHandling(414);
            return;
        }
        std::string requestLine = stashStr.substr(0, requestLineSize);
        code_t  parsingResult = _fillInRequestLineInfo(requestLine); //actual parsing is done here
        if (parsingResult != 200) {
            _abortRequestHandling(parsingResult);
            return;
        } else {
            _socket.consumeRecvStash(requestLineSize + std::string(DELIMITOR).length());
            std::cerr << "Completed parsing of request line" << std::endl;
            _stateMachine.advanceState();
        }
    } else if (stash.size() > BUFFER_SIZE) { //stash too big to hold a line (first "if" already checked if line is shorter)
        _abortRequestHandling(414);
        return;
    } else {
        std::cerr << "lol" << std::endl;
        return; //Returns without modification if any of the above isn't true, as it needs to wait for the next read
    }
    
}
void	RequestHandler::_parseHeaders(void)
{
    const size_t    delimitorSize = std::string(DELIMITOR).length();
    const Socket::data_container_t&    stash = _socket.getRecvStash();

    std::string stashStr(stash.begin(), stash.end());
    static size_t   headersSize = 0;
    
    while (stashStr.compare(0, 2, DELIMITOR) != 0) {// As headers are consumed dinamically, delimitor of header section will be at the beginning
        size_t  fieldEnd = stashStr.find(DELIMITOR);
        if (fieldEnd == stashStr.npos) {
            return; //Header field is not complete, so a new recv must be done before continuing parsing
        }
        std::string field = stashStr.substr(0, fieldEnd);
        code_t      parsingResult = _parseHeaderField(field);
        if (parsingResult != 200) {
            _abortRequestHandling(parsingResult);
            return;
        }
        _socket.consumeRecvStash(fieldEnd + delimitorSize);
        stashStr.erase(0, delimitorSize);
        headersSize += fieldEnd + delimitorSize;
        if (headersSize > BUFFER_SIZE * 4) {
            _abortRequestHandling(431);
            return;
        }
    }
    //Since all conditions inside the while loop have returns, this section is only reached
    // if the end of the header section is reached, so the state must be advanced
    _stateMachine.advanceState();
}
void	RequestHandler::_processRequest(void)
{
    //Check return code
    int  output = _serverSettings.getReturnCode();
    if (output != -1) { // Means that a return code is present
        code_t  returnCode = static_cast<code_t>(output);
        bool    isRedirectCode = _serverSettings.getReturnCode() >= 300
                                    && _serverSettings.getReturnCode() < 400;
        if (isRedirectCode) {
            _response.addHeader("Location", _serverSettings.getReturnURL());
        }
        _abortRequestHandling(returnCode);
        return;
    }
    //Check if method is allowed
    if (_serverSettings.getAllowMethod(_request.getMethod()) == false) {
		_abortRequestHandling(405);
        return;
	}
    
    //build full target
	std::string	target(_serverSettings.getRoot());
	target.append(_request.getTarget());
	_request.setTarget(target);

	// Processing based on header fields
	const HttpMessage::headers_dict&	headers = _request.getHeaders();
    // Method is POST, but no header indicates existence of a body
    bool    postWithNoBody = (_request.getMethod() == POST
		&& headers.find("Content-Length") == headers.end()
		&& headers.find("Transfer-Encoding") == headers.end());
	if (postWithNoBody) {
		_abortRequestHandling(400);
	}
    // Set bodysize
	if (headers.find("Content-Length") != headers.end())
	{
		std::string	sizeStr = headers.at("Content-Length");
		ssize_t bodySize = std::atoi(sizeStr.c_str()); //TODO Should check if it is an integer
		if (bodySize > static_cast<ssize_t>(_serverSettings.getClientBodySize()))
			_abortRequestHandling(413);
		_request.setBodySize(bodySize);
	}
	_stateMachine.advanceState();
}
void	RequestHandler::_consumeBody(void)
{
    //TODO
}
void	RequestHandler::_sendResponseHeader(void)
{
    std::stringstream response;    
    //Status line:
    response << "HTTP/1.1 " << _response.getStatusCode() << DELIMITOR;
    //Headers:
    const HttpMessage::headers_dict headers = _response.getHeaders();
    HttpMessage::headers_dict::const_iterator cit;
    for (cit = headers.begin(); cit != headers.end(); cit++)
        response << cit->first << ": " << cit->second << DELIMITOR;
    response << DELIMITOR;
    std::string sendBuff(response.str());
    _socket.addToSendStash(reinterpret_cast<const Socket::byte_t*>(sendBuff.c_str()), sendBuff.length());;
    _stateMachine.advanceState();
}
void	RequestHandler::_sendResponseBody(void)
{
    //TODO must complete
	std::stringstream response;
	std::ifstream file(_response.getBodyPath().c_str(), std::ios::binary);
    if (file.fail()) {
        std::cerr << "DAMN IT" << std::endl;
    } else {
	    response << file.rdbuf();
        file.close();
    }
    response << "\r\n"; //TODO: this line must only run if body is fully sent
	std::string	sendBuff = response.str(); 
    _socket.addToSendStash(reinterpret_cast<const Socket::byte_t*>(sendBuff.c_str()), sendBuff.length());
    if (_socket.getSendStash().size() == 0) {
        _stateMachine.advanceState(); //TODO: this line must only run if body is fully sent
    }
}


void	RequestHandler::_abortRequestHandling(code_t statusCode)
{
	std::cerr << "ABORT! error code: " << statusCode << std::endl;
	_response.setStatusCode(statusCode, _serverSettings.getErrorPage(statusCode));
	switch (statusCode) {
        case 400:
        case 404: //TODO put the rest of the close connection codes
            std::cerr << "loladas " << std::endl;
            _setCloseConnection(true);
            _socket.clearStashes();
            _stateMachine.setCurrentState(RESPONDING_HEADER);
            break;
        default:
            _stateMachine.setCurrentState(CONSUME_BODY);
    }
}

RequestHandler::code_t  RequestHandler::_fillInRequestLineInfo(std::string requestLine)
{
    //Initial sanitizer
	code_t	sanitizerResult = _httpSanitizer(requestLine);
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

RequestHandler::code_t	RequestHandler::_parseHeaderField(std::string headerField)
{
	std::string	fieldName;
	std::string	fieldValue;
	
	code_t  sanitizerResult = _httpSanitizer(headerField);
    if (sanitizerResult != 200){
        return (sanitizerResult);
    }
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

	//Match the server and location as soon as possible
	if (fieldName.compare("Host") == 0)
	{
		std::size_t	colonPos = fieldValue.find(':');
		_serverSettings.setServer(std::string(fieldValue, 0, colonPos));
		_serverSettings.setLocation(_request.getTarget());
	}
	//Place the header in the request.
	_request.addHeader(fieldName, fieldValue);
    return (200);
}

RequestHandler::code_t	RequestHandler::_httpSanitizer(std::string line)
{
	size_t  crPos = line.find('\r');;
    do
    {
        if (crPos != line.npos && line[crPos + 1] != '\n')
            return (400);
        crPos++;
        crPos = line.find('\r', crPos);
    } while (crPos != line.npos);
	return (200);
}

Method  RequestHandler::_strToMethod(std::string str)
{
    const std::string methods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++)
	{
		if (str.compare(0, methods[i].length() + 1, methods[i] + " ") == 0)
			return (static_cast<Method>(i));
	}
	return (UNKNOWN);
} */