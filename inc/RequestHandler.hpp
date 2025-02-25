/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:40:39 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/25 19:25:51 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include "StateMachine.hpp"

class   RequestHandler
{
public:
    RequestHandler();
    ~RequestHandler();

    void    handle(void);

    bool    isReceiving(void);
    bool    isSending(void);
    bool    isDone(void);
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
    StateMachine<State>		_stateMachine;
};


#endif