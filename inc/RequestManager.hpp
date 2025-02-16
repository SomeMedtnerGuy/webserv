/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:09:45 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 10:22:30 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_MANAGER_HPP
# define REQUEST_MANAGER_HPP

# include <cstdlib> // For abort(). Must be removed in production
# include <iostream>

# include "Socket.hpp"
# include "StateMachine.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "HttpParser.hpp"

// This class will receive, process and answer a request when one is detected to exist in a socket
// An instance of this class should be created each time a request is detected by its caller
class RequestManager
{
public:
	typedef unsigned int code;
	
	RequestManager(Socket& socket);
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
		PROCESSING,
		RESPONDING,
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

	

	Socket&				_socket;
	HttpRequest			_request;
	HttpResponse		_response;

	void	_parseRequestLine(void);
	void	_parseHeaders(void);
	bool	_processRequest(void);
	bool	_sendResponse(void);

	void	_abortRequestHandling(unsigned int errorCode);
};

#endif