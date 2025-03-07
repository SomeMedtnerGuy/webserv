/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:37:03 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/07 13:49:12 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# define BUFFER_SIZE 8192

# include <poll.h>
# include <vector>
# include <netinet/in.h> //for recv() and send()
# include <cstring> //for memset()
# include <iostream>
# include <algorithm> //for min()

class   Socket
{
public:
    enum Action {
        RECV,
        SEND
    };
    typedef struct pollfd       sockfd_t;
    typedef unsigned char       byte_t;
    typedef std::vector<byte_t>	data_container_t;
    
    Socket(sockfd_t& sockfd);
    ~Socket();
    
    int                     getSockFd(void) const;
    void                    updateFlags(void);
    const data_container_t& getRecvStash(void) const;
    void                    consumeRecvStash(size_t byteAm);
    const data_container_t& getSendStash(void) const;
    void                    addToSendStash(const data_container_t bytes);
    void                    clearStashes(void);
    bool                    canRecv(void) const;
    bool                    canSend(void) const;
    void                    fillStash(void);
    void                    flushStash(void);

    class SocketException: public std::exception
    {
    public:
        enum ActionReturn {
            ERROR = -1,
            CONN_CLOSED = 0
        };
        SocketException(Action action, ActionReturn actionReturn);
        ~SocketException() throw();
        const char* what() const throw();
    private:
        Action          _action;
        ActionReturn    _actionReturn;
        std::string     _message;
    };

private:
    sockfd_t&         _sockfd;
    byte_t      	  _buffer[BUFFER_SIZE];
    data_container_t  _recvStash;
    data_container_t  _sendStash;
    bool              _canRecv;
        void    _setCanRecv(bool canRecv);
        bool    _getCanRecv(void) const;
    bool            _canSend;
        void    _setCanSend(bool canSend);
        bool    _getCanSend(void) const;
};

#endif