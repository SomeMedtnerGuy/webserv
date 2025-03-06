/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:20:49 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/05 08:46:31 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"

PollManager* PollManager::_instance = NULL;

PollManager::PollManager(){}
PollManager::~PollManager(){delete _instance;}

PollManager* PollManager::getInstance()
{
    if (_instance == NULL)
        _instance = new PollManager;
    return _instance;
}

void PollManager::destroyInstance() {
    if (_instance != NULL) {
        delete _instance;
        _instance = NULL;
    }
}

void PollManager::removeDescriptor(int fd)
{
	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it) 
	{
        if (it->fd == fd) 
		{
            _fds.erase(it);
            return;
        }
    }
}

std::vector<pollfd>& PollManager::getPollFds(){return (_fds);}

