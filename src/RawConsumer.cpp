/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawConsumer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:33:10 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/12 18:51:30 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawConsumer.hpp"
#include <ctime>

RawConsumer::RawConsumer(HttpResponse&  response, bool shouldPerformPost, std::string saveFileName, size_t bodySize)
    : ABodyConsumer(response, shouldPerformPost), _bodySize(bodySize)
{
    if (_shouldPerformPost) {
        int i = 0;
        if (saveFileName.find("cgi-bin") != std::string::npos)
        {
            std::srand(std::time(0));
            std::ostringstream fileNameStream;
        
            fileNameStream << "root/cgi-bin/cgifile";
            fileNameStream << std::rand();
            
            response.cgiFile = fileNameStream.str();
            saveFileName = fileNameStream.str();
        }
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
    }
}
RawConsumer::~RawConsumer(){}

size_t  RawConsumer::consume(data_t& data)
{
    size_t  bytesAm = std::min(data.size(), _bodySize);
    if (_shouldPerformPost) {
        _saveFile.write(reinterpret_cast<char*>(data.data()), bytesAm);
    }
    _bodySize -= bytesAm;
    if (_bodySize == 0) {
        if (_bodySize == 0 && data.size() > bytesAm) {
            _response.setStatusCode(400);
        }
        _setIsDone(true);
    }
    return (bytesAm);
}