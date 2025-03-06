/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:52:36 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/04 16:26:44 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include "StateMachine.hpp"
# include "Socket.hpp"
# include "ConfigFile.hpp"
# include "RequestParser.hpp"
# include "RequestPerformer.hpp"
# include "ResponseSender.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class RequestManager
{
public:
    typedef HttpResponse::code_t    code_t;
    
    RequestManager(Socket& socket, ConfigFile& configFile);
    ~RequestManager();

    void    handle(void);
    bool    isDone(void) const;
    bool    shouldCloseConnection(void) const;
private:
    enum State {
        RECV_HEADER,
        RECV_BODY,
        SEND_RESPONSE,
        STATE_AM
    };
    StateMachine<State> _stateMachine;

    enum ErrorSeverity {
        NO_ERROR,
        CONSUME_AND_ANSWER,
        ANSWER_AND_CLOSE,
        CLOSE_IMMEDIATELY,
        ERROR_SEVERITY_AM
    };
    
    Socket&             _socket;
    ServerSettings      _serverSettings;
    

    RequestParser       _requestParser;
    RequestPerformer    _requestPerformer;
    
    ResponseSender      _responseSender;
    

    HttpRequest			_request;
	HttpResponse		_response;

    bool	_handlingComplete;
		void	_setHandlingComplete(bool value);
		bool	_getHandlingComplete(void) const;
	bool	_closeConnection;
		void	_setCloseConnection(bool value);
		bool	_getCloseConnection(void) const;
    
    void            _checkAndActOnErrors(void);
    ErrorSeverity   _getErrorSeverity(code_t statusCode);
};


#endif