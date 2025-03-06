/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawPoster.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:33:10 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 15:36:46 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawPoster.hpp"

RawPoster::RawPoster(HttpResponse&  response, std::string saveFileName, size_t bodySize)
    : APostPerformer(response), _bodySize(bodySize)
{
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
}
RawPoster::~RawPoster(){}

size_t  RawPoster::post(data_t& data)
{
    size_t  bytesAm = std::min(data.size(), _bodySize);
    _saveFile.write(reinterpret_cast<char*>(data.data()), bytesAm);
    _bodySize -= bytesAm;
    bool readyToBeDone = _bodySize == 0
                            && data.size() >= 2;
    bool endIsCorrect = readyToBeDone
                            && data.data()[0] == '\r'
                            && data.data()[1] == '\n';
    if (readyToBeDone) {
        if (endIsCorrect) {
            bytesAm += 2; 
        } else {
            _response.setStatusCode(400);
        }
        _setIsDone(true);
    }
    return (bytesAm);
}