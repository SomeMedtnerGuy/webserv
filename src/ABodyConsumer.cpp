/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABodyConsumer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:53:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/12 09:37:37 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABodyConsumer.hpp"

ABodyConsumer::ABodyConsumer(HttpResponse& response, bool shouldPerformPost)
    :_response(response), _shouldPerformPost(shouldPerformPost), _isDone(false)
{}
ABodyConsumer::~ABodyConsumer(){}

bool    ABodyConsumer::isDone() {return (_getIsDone());}

void    ABodyConsumer::_setShouldPerformPost(bool newValue){_shouldPerformPost = newValue;}
bool    ABodyConsumer::_getShouldPerformPost(void) const {return (_shouldPerformPost);}
void    ABodyConsumer::_setIsDone(bool newValue){_isDone = newValue;}
bool    ABodyConsumer::_getIsDone(void) const {return (_isDone);}