/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:21:16 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/15 14:26:29 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# define BUFFER_SIZE 8192

# include <vector>
# include <cstring> //for memset
# include <sys/socket.h> //for recv
# include <exception>
# include <iostream>

// This class is a wrapper to a socket. It allows reading from and writing to it
// as well as holding some info such as if those operations are possible or not an a stash for unprocessed data.
class Socket
{
public:
	typedef std::vector<unsigned char>	data_container;

	Socket(int sockfd);
	~Socket();

	//Control the state of the socket. All reads and writes will be checked internally using these variables.
	void	setCanRead(bool value);
	void	setCanWrite(bool value);
	void	resetState(void);

	const data_container	getStash(void) const;
	void					consumeFromStash(size_t bytesAmount);

	// I need to know if the read was successful, hence the return
	ssize_t	readToStash(void);
	

private:
	const int		_sockfd;
	unsigned char	_buffer[BUFFER_SIZE];
	data_container	_stash;
	
	bool		_canRead;
	bool		_canWrite;
	
	
	bool	_getCanRead(void) const;
	bool	_getCanWrite(void) const;
};

#endif