/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawPoster.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:33:10 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/08 19:43:03 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawPoster.hpp"
#include <ctime>

RawPoster::RawPoster(HttpResponse&  response, std::string saveFileName, size_t bodySize)
    : APostPerformer(response), _bodySize(bodySize)
{
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
}
RawPoster::~RawPoster(){}

size_t  RawPoster::post(data_t& data)
{
    size_t  bytesAm = std::min(data.size(), _bodySize);
    _saveFile.write(reinterpret_cast<char*>(data.data()), bytesAm);
    _bodySize -= bytesAm;
    // bool readyToBeDone = _bodySize == 0
    //                         && data.size() >= 2;
    // bool endIsCorrect = readyToBeDone
    //                         && data.data()[0] == '\r'
    //                         && data.data()[1] == '\n';
    // if (readyToBeDone) {
    //     if (endIsCorrect) {
    //         bytesAm += 2; 
    //     } else {
    //         std::cout << "error 400" << std::endl;
    //         _response.setStatusCode(400);
    //     }
    //     _setIsDone(true);
    // }
    // return (bytesAm);

    if (_bodySize == 0) {
        if (_bodySize == 0 && data.size() > bytesAm) {
            _response.setStatusCode(400);
        }
        _setIsDone(true);
    }
    return (bytesAm);
}