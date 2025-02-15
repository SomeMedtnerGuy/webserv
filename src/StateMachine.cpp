/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateMachine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:43:14 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 15:34:13 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StateMachine.hpp"

StateMachine::StateMachine(int stateAm): _statesAm(stateAm), _currentState(0) {};
StateMachine::~StateMachine(){}

void	StateMachine::advanceState() {
	setCurrentState(getCurrentState() + 1);
}
void	StateMachine::setCurrentState(unsigned int newState) {
	_currentState = newState % _statesAm;
}
unsigned int	StateMachine::getCurrentState(void) const {return (_currentState);}