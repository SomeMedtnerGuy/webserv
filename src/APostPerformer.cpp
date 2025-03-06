/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APostPerformer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:53:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/05 14:00:13 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APostPerformer.hpp"

APostPerformer::APostPerformer(HttpResponse& response)
    :_response(response), _isDone(false)
{}
APostPerformer::~APostPerformer(){}

bool    APostPerformer::isDone() {return (_getIsDone());}

void    APostPerformer::_setIsDone(bool newValue){_isDone = newValue;}
bool    APostPerformer::_getIsDone(void) const {return (_isDone);}