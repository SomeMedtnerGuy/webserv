/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:11:54 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/05 08:45:58 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLMANAGER_HPP
#define POLLMANAGER_HPP

# include <iostream>
# include <vector>
# include <poll.h>

class PollManager {
private:
	std::vector<pollfd> _fds;
	static PollManager* _instance;
	
	PollManager();
	PollManager(const PollManager&);
    PollManager& operator=(const PollManager&);

public:
	static PollManager* getInstance();
	static void destroyInstance();
	
	void addDescriptor(int fd, short events);
	void removeDescriptor(int fd);
	
	std::vector<pollfd>& getPollFds();
	
	
	~PollManager();
};

#endif // POLLMANAGER_HPP