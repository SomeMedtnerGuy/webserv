/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OldRequestHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:40:39 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/27 17:04:25 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# define DELIMITOR "\r\n"
# define TOKEN_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&'*+-.^_`|~"

# include <iostream>

# include "StateMachine.hpp"
# include "Socket.hpp"
# include "ServerSettings.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class   RequestHandler
{
public:
	typedef HttpResponse::code_t code_t;

    RequestHandler(Socket& socket, ConfigFile& configFile);
    ~RequestHandler();

    void    handle(void);

    bool    isReceiving(void) const;
    bool    isSending(void) const;
    bool    isDone(void) const;
	bool	shouldCloseConnection(void) const;
private:
    enum State {
		PARSING_REQUEST_LINE,
		PARSING_HEADERS,
		PROCESSING_REQUEST,
		CONSUME_BODY,
		RESPONDING_HEADER,
		RESPONDING_BODY,
		DONE,
		STATE_SIZE
	};
    StateMachine<State>		_stateMachine;

	bool	_handlingComplete;
		void	_setHandlingComplete(bool value);
		bool	_getHandlingComplete(void) const;
	bool	_closeConnection;
		void	_setCloseConnection(bool value);
		bool	_getCloseConnection(void) const;

	//Objects used for parsing
	Socket&				_socket;
	ServerSettings		_serverSettings;
	HttpRequest			_request;
	HttpResponse		_response;

	//Main parsing routines
	void	_parseRequestLine(void);
	void	_parseHeaders(void);
	void	_processRequest(void);
	void	_consumeBody(void);
	void	_sendResponseHeader(void);
	void	_sendResponseBody(void);

	code_t	_httpSanitizer(std::string line);
	code_t	_fillInRequestLineInfo(std::string requestLine);
	code_t	_parseHeaderField(std::string headerField);
	void	_abortRequestHandling(code_t statusCode);
	Method	_strToMethod(std::string str);

};


#endif*/