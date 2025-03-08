/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:30:00 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/03 16:06:33 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse(): _statusCode(200), _bodyPath(DEFAULT_PAGE_PATH) 
{
	_headers["Content-Type"] = "text/html; charset=UTF-8";
    _headers["Server"] = "localhost";
    _headers["Content-Length"] = ntostr(getFileLength(_bodyPath));
}
HttpResponse::~HttpResponse() {}

void	HttpResponse::setStatusCode(code_t statusCode, std::string errorPagePath)
{
	_statusCode = statusCode;
	setBodyPath(errorPagePath);
}
HttpResponse::code_t	HttpResponse::getStatusCode(void) const {return (_statusCode);}
void						HttpResponse::setBodyPath(std::string bodyPath)
{
	_bodyPath = bodyPath;

	_headers["Content-Length"] = ntostr(getFileLength(_bodyPath));
	for (header_map::const_iterator cit = _fileTypeMap.begin();
			cit != _fileTypeMap.end(); cit++) {
		if (bodyPath.find(cit->first) != bodyPath.npos) {
			_headers["Content-Type"] = cit->second;
			break;
		}
	}
	//It is possible to tell the browser to download the file directly by adding the following header:
	//'Content-Disposition = attachment; filename="<filename>"' where <filename> is the filename.
	//A possible feature is to add this header to any bodyPath that does not have the ".html" extension
}
std::string					HttpResponse::getBodyPath(void) const {return (_bodyPath);}

void	HttpResponse::printMessage(void)
{
	std::cerr << "Status code: " << _statusCode << std::endl;
	
	std::cerr << "Body path: " << _bodyPath << std::endl;
	
	std::cerr << "Headers:" << std::endl;
	for (headers_dict::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		std::cerr << it->first << ": " << it->second << std::endl;
	}
}

const HttpResponse::header_map	HttpResponse::_createFileTypeMap(void)
{
	header_map	m;
	m[".jpg"] = "image.jpeg";
	m[".pdf"] = "application/pdf";
	return (m);
}
const HttpResponse::header_map	HttpResponse::_fileTypeMap = HttpResponse::_createFileTypeMap();
