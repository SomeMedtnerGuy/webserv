/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkedPoster.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:58:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/05 12:37:09 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNKED_POSTER_HPP
# define CHUNKED_POSTER_HPP

# include "APostPerformer.hpp"
# include "StateMachine.hpp"
# include <cstdlib> //for strtol
# include <fstream>

class ChunkedPoster: public APostPerformer
{
public:
    ChunkedPoster(HttpResponse&  response, std::string saveFileName);
    ~ChunkedPoster();

    size_t  post(data_t& data);
private:
    enum State {
        CHUNK_SIZE,
        CHUNK,
        STATE_AM
    };
    StateMachine<State> _stateMachine;

    data_t          _data;
    size_t          _currentChunkSize;
    std::ofstream  _saveFile;  
    
    void    _parseChunkSize(void);
    void    _parseChunk(void);
};

#endif