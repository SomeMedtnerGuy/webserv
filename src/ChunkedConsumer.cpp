/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkedConsumer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:13:47 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/13 11:14:40 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkedConsumer.hpp"

ChunkedConsumer::ChunkedConsumer(HttpResponse&  response, bool shouldPerformPost/*, std::string saveFileName*/)
    : ABodyConsumer(response, shouldPerformPost), _stateMachine(STATE_AM, CHUNK_SIZE), _currentChunkSize(0)
{
    /*if (_shouldPerformPost) {
        int i = 0;
        while (isFile(saveFileName)) {
            saveFileName.insert(saveFileName.find_last_of('.'), "(1)");
            i++;
            if (i >= 100) {
                _response.setStatusCode(409);
                return ;
            }
        }
        _saveFile.open(saveFileName.c_str(), std::ios::binary);
        if (_saveFile.fail()) {
            _response.setStatusCode(500);
        }
        _response.cgiFile = saveFileName;
    }*/
}
ChunkedConsumer::~ChunkedConsumer()
{
    _saveFile.close();
}

size_t  ChunkedConsumer::consume(data_t& data)
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

void    ChunkedConsumer::_parseChunkSize(void)
{   
    std::string dataStr(_data.begin(), _data.begin() + std::min(_data.size(), size_t(16)));
    //printString(dataStr);
    //std::cerr << std::endl;
    if (dataStr.find("0") == 0) { //Indication that body will be done
        if (dataStr.length() < 5) {
            return;
        }
        _data.erase(_data.begin(), _data.begin() + 5);
        _setIsDone(true);
        return;
    }
    size_t  endPosOfSize = dataStr.find("\r\n");
    if (endPosOfSize == dataStr.npos) {
        if (_data.size() < 16) {
            return ; //Still hasnt received entire thing
        }
        _response.setStatusCode(400);
        _setIsDone(true);
        return;
    }
    char*   endPointer;
    _currentChunkSize = std::strtol(dataStr.c_str(), &endPointer, 16);
    if (endPointer == dataStr.c_str()
        || size_t(endPointer - dataStr.c_str()) != endPosOfSize) { //No conversion was possible, so request is malformed
        _response.setStatusCode(400); 
        _setIsDone(true);
        return;
    }
    _data.erase(_data.begin(), _data.begin() + endPosOfSize + 2);
    _stateMachine.advanceState();
}

void    ChunkedConsumer::_parseChunk(void)
{
    size_t  maxBytesPossible = std::min(_data.size(), _currentChunkSize);
    size_t  bytesToSave = std::min(size_t(BUFFER_SIZE), maxBytesPossible);
    if (_shouldPerformPost) {
        _saveFile.write(reinterpret_cast<char*>(_data.data()), bytesToSave);
    }
    _currentChunkSize -= bytesToSave;
    _data.erase(_data.begin(), _data.begin() + bytesToSave);
    if (_currentChunkSize == 0) {
        if (_data.size() < 2) {
            return ;
        }
        _data.erase(_data.begin(), _data.begin() + 2);
        _stateMachine.advanceState();
    }
}