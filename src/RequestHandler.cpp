/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:03:27 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/25 19:28:42 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler(): _stateMachine(STATE_SIZE, PARSING_REQUEST_LINE) {};
RequestHandler::~RequestHandler() {};

void    RequestHandler::handle(void)
{
    //TODO
}

bool    RequestHandler::isReceiving(void)
{
    State   currentState = _stateMachine.getCurrentState();
    switch (currentState) {
        case PARSING_REQUEST_LINE:
        case PARSING_HEADERS:
        case PROCESSING_REQUEST:
        case PERFORMING_REQUEST:
            return (true);
            break;
        default:
            return (false);
    }
}

bool    RequestHandler::isSending(void)
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

bool    RequestHandler::isDone(void)
{
    return (_stateMachine.getCurrentState() == DONE);
}