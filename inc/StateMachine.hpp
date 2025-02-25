/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateMachine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:37:12 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/25 19:18:07 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_MACHINE_HPP
# define STATE_MACHINE_HPP

template <typename T>
class StateMachine
{
public:
	StateMachine(int statesAm, T startingState)
		: _statesAm(statesAm), _currentState(startingState) {}
	~StateMachine() {}

	void			advanceState(void) {
		setCurrentState(getCurrentState() + 1);
	}
	void			setCurrentState(T newState) {
		_currentState = newState % _statesAm;
	}
	T				getCurrentState(void) const {
		return (_currentState);
	}

private:
	const int		_statesAm;
	T	_currentState;
};

#endif