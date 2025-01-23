/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:44:30 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/23 17:08:08 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include "ConfigFile.hpp"
# include "ServerSettings.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

# include "RequestParser.hpp"
# include "RequestProcessor.hpp"

# include <cstring>

/* FLAGS */
# define HEADER_PROCESSED	0b1
# define BODY_EXISTS		0b10
# define REQUEST_PROCESSED	0b100
# define REQUEST_HANDLED	0b1000

class RequestHandler {
private:
	int	_sockfd; // The socket used to communicate with client
	int	_flags; // Keep track of in which step the handler is in

	ServerSettings	_serverSettings;
	HttpRequest		_requestObj;
	HttpResponse	_responseObj;

	unsigned char	_buffer[BUFFER_SIZE + 1];
	std::vector<unsigned char>	_stash;

	RequestHandler();
	RequestHandler(const RequestHandler& other);
	RequestHandler&	operator=(const RequestHandler& other);

	void	_setFlags(int flags);
	void	_unsetFlags(int flags);
	int		_getFlags(void) const;
	bool	_isFlagOn(int flag) const;

	std::string	_receiveRequestHeader(void);
	void        _sendResponse(void);
	
	
public:
	RequestHandler(int sockfd, ConfigFile& configs);
	~RequestHandler();

	void	handleRequest(void);
	bool	isRequestHandled(void);

	/* Exceptions */
	class RecvSendError: public std::exception {
	private:
		int	_errorCode;
		RecvSendError();
	public:
		RecvSendError(int errorCode);
		~RecvSendError() throw();
		int	getErrorCode(void) const throw();
	};
	
	class HttpError: public std::exception {
	private:
		int	_statusCode;
		HttpError();
	public:
		HttpError(int statusCode);
		~HttpError() throw();
		int	getStatusCode(void) const throw();
	};
};

#endif