/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateMachine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:37:12 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 15:30:31 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_MACHINE_HPP
# define STATE_MACHINE_HPP

class StateMachine
{
public:
	StateMachine(int statesAm);
	~StateMachine();

	void			advanceState(void);
	void			setCurrentState(unsigned int state);
	unsigned int	getCurrentState(void) const;

private:
	const int		_statesAm;
	unsigned int	_currentState;
};

#endif