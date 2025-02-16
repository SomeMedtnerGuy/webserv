/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:09:45 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 22:50:03 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_MANAGER_HPP
# define REQUEST_MANAGER_HPP

# include <cstdlib> // For abort(). Must be removed in production
# include <iostream>
# include <dirent.h>

# include "ConfigFile.hpp"
# include "ServerSettings.hpp"
# include "Socket.hpp"
# include "StateMachine.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "utils.hpp"

# define TOKEN_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&'*+-.^_`|~"

// This class will receive, process and answer a request when one is detected to exist in a socket
// An instance of this class should be created each time a request is detected by its caller
class RequestManager
{
public:
	typedef unsigned int code;
	
	RequestManager(Socket& socket, ConfigFile& configFile);
	~RequestManager();
	
	// The function that should be called in a loop until the handling is complete.
	void	handleMoreRequest(void);
	// Tells the caller whether after the most recent iteration, the handling is complete.
	bool	isHandlingComplete(void) const;
	// Tells the caller whether after the most recent iteration, the connection should be closed.
	bool	shouldCloseConnection(void) const;

private:
	enum State {
		PARSING_REQUEST_LINE,
		PARSING_HEADERS,
		PROCESSING_REQUEST,
		PERFORMING_REQUEST,
		RESPONDING_HEADER,
		RESPONDING_BODY,
		DONE,
		STATE_SIZE
	};
	StateMachine		_stateMachine;

	bool	_closeConnection;
		void	_setCloseConnection(bool value);
		bool	_getCloseConnection(void) const;
	bool	_handlingComplete;
		void	_setHandlingComplete(bool value);
		bool	_getHandlingComplete(void) const;

	//Objects used for parsing
	Socket&				_socket;
	ServerSettings		_serverSettings;
	HttpRequest			_request;
	HttpResponse		_response;

	//Main parsing routines
	void	_parseRequestLine(void);
	void	_parseHeaders(void);
	void	_processRequest(void);
	void	_performRequest(void);
	void	_sendResponseHeader(void);
	void	_sendResponseBody(void);

	void	_abortRequestHandling(HttpResponse::status_code statusCode);

	//Parsing sub-routines
	HttpResponse::status_code	_httpSanitizer(std::string requestLine);
	HttpResponse::status_code	_parseRequestLine(std::string requestLine);
	HttpResponse::status_code	_parseHeaderField(std::string headerField);
	
	void	_performGet(void);
	void	_performDelete(void);
	void	_performPost(void);

	void			_createAutoIndex(std::string target);
	static Method	_strToMethod(std::string methodStr); //TODO put in utils lol
};

#endif