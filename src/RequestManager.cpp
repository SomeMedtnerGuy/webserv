/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:06:46 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 19:04:01 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestManager.hpp"

/* PUBLIC */
RequestManager::RequestManager(Socket& socket)
	: _stateMachine(STATE_SIZE), _socket(socket) {}
RequestManager::~RequestManager() {}

void	RequestManager::handleMoreRequest(void)
{
	switch (_stateMachine.getCurrentState()) {
		case PARSING_REQUEST_LINE:
			_parseRequestLine();
		case PARSING_HEADERS:
			_parseHeaders();
		case PROCESSING:
			_processRequest();
		case RESPONDING:
			_sendResponse();
		case DONE:
			_setHandlingComplete(true);
		default:
			//TODO: Remove before production
			std::cerr << "Some unexpected state happened at RequestManager. Aborting" << std::endl;
			std::abort(); 
	}
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
		size_t	endOfRequestLine = requestLine.find("\r\n");
		// if it is found
		if (endOfRequestLine != requestLine.npos && endOfRequestLine <= BUFFER_SIZE)
		{
			//process it, consume it, advance state and return
			code parsingResult = HttpParser::parseRequestLine(_request, requestLine);
			if (parsingResult != 200)
				_abortRequestHandling(parsingResult);
			else {
				_socket.consumeFromStash(requestLine.length());
				_stateMachine.advanceState();
			}
			return;
		}
		// else if the request line becomes bigger than it should
		else if (endOfRequestLine > BUFFER_SIZE)
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
	//TODO
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
		code	parsingResult = HttpParser::parseHeaderField(_request, field); //TODO: implement plz
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

bool	RequestManager::_processRequest(void)
{
	//TODO
}

bool	RequestManager::_sendResponse(void)
{
	//TODO
}

void	RequestManager::_abortRequestHandling(unsigned int errorCode)
{
	//TODO
	//Build response
	//Check if error is bad enough to close connection
	//Change state directly to sending response
}