/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:44:30 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/27 13:50:50 by ndo-vale         ###   ########.fr       */
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
# define HEADER_PROCESSED		0b1
# define BODY_EXISTS			0b10
# define METHOD_PERFORMED		0b100
# define REQUEST_PROCESSED		0b1000
# define REQUEST_HANDLED		0b10000
# define CLOSE_CONNECTION		0b100000

class RequestHandler {
private:
	int		_sockfd; // The socket used to communicate with client
	int		_flags; // Keep track of in which step the handler is in
	bool	_closeConnection; //Informs whether connection must be closed immediately

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

	void		_performGet(void);
	void		_performDelete(void);
	void		_performPost(void);
	
	void		_readSomeBody(void);
	void		_createAutoIndex(std::string target);
	
	
public:
	RequestHandler(int sockfd, ConfigFile& configs);
	~RequestHandler();

	//Returns. Should be used to close connection if required
	void	handleRequest(void);
	bool	isRequestHandled(void);
	bool	shouldCloseConnection(void);

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