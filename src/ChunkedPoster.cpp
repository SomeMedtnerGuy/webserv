/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkedPoster.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:13:47 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/05 14:48:39 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkedPoster.hpp"

ChunkedPoster::ChunkedPoster(HttpResponse&  response, std::string saveFileName)
    : APostPerformer(response), _stateMachine(STATE_AM, CHUNK_SIZE), _currentChunkSize(0)
{
    _saveFile.open(saveFileName.c_str(), std::ios::binary);
}
ChunkedPoster::~ChunkedPoster()
{
    _saveFile.close();
}

size_t  ChunkedPoster::post(data_t& data)
{
    _data = data;
    State   currentState = _stateMachine.getCurrentState();
    State   previousState = currentState;
    do {
        switch (currentState) {
            case CHUNK_SIZE:
                _parseChunkSize();
                break;
            case CHUNK:
                _parseChunk();
                break;
            default:
                std::cerr << "Some unexpected state happened!" << std::endl;
                throw (std::exception());
        }
        previousState = currentState;
        currentState = _stateMachine.getCurrentState();
    } while (currentState != previousState);
    return (data.size() - _data.size());
}

void    ChunkedPoster::_parseChunkSize(void)
{
    std::cerr << "This shit should be called" << std::endl;
    std::cerr << "Data size: " << _data.size() << std::endl;
    std::string dataStr(_data.begin(), _data.begin() + std::min(_data.size(), size_t(16)));
    std::cerr << std::endl;
    //for (size_t i = 0; i < _data.size(); i++) {
    //    std::cerr << _data[i];
    //}
    //std::cerr << std::endl << std::endl;
    std::cerr << "str: " << dataStr << std::endl;
    if (dataStr.find("0\r\n\r\n") == 0) { //Indication that body is done
        std::cerr << "1" << std::endl;
        _data.erase(_data.begin(), _data.begin() + 5);
        _setIsDone(true);
        return;
    }
    size_t  endPosOfSize = dataStr.find("\r\n");
    if (endPosOfSize == dataStr.npos) {
        if (_data.size() < 16) {
            return ;
        }
        _response.setStatusCode(400, ""); //TODO Take care of this. Must give response access to serverSettings somehow
        _setIsDone(true);
        std::cerr << "2" << std::endl;
        return;
    }
    char*   endPointer;
    _currentChunkSize = std::strtol(dataStr.c_str(), &endPointer, 16);
    std::cerr << "AAAAAAAAAAAAAAAAAAAAAAAAAAA " << _currentChunkSize << std::endl;
    if (endPointer == dataStr.c_str()
        || size_t(endPointer - dataStr.c_str()) != endPosOfSize) { //No conversion was possible, so request is malformed
        _response.setStatusCode(400, ""); //TODO Take care of this. Must give response access to serverSettings somehow
        _setIsDone(true);
        std::cerr << "3" << std::endl;
        return;
    }
    std::cerr << "4" << std::endl;
    _data.erase(_data.begin(), _data.begin() + endPosOfSize + 2);
    std::cerr << _currentChunkSize << std::endl;
    _stateMachine.advanceState();
}

void    ChunkedPoster::_parseChunk(void)
{
    std::cerr << "data size: " << _data.size() << std::endl << "chunk size: " << _currentChunkSize << std::endl;

    size_t  maxBytesPossible = std::min(_data.size(), _currentChunkSize);
    size_t  bytesToSave = std::min(size_t(BUFFER_SIZE), maxBytesPossible);
    _saveFile.write(reinterpret_cast<char*>(_data.data()), bytesToSave);
    _currentChunkSize -= bytesToSave;
    _data.erase(_data.begin(), _data.begin() + bytesToSave);
    if (_currentChunkSize == 0) {
        _data.erase(_data.begin(), _data.begin() + 2);
        _stateMachine.advanceState();
    }
}