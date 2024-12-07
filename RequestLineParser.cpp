/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:45:14 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/12/07 19:47:21 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLineParser.hpp"

const std::string	RequestLineParser::_methods[3] = {"GET", "POST", "DELETE"};

RequestLineParser::RequestLineParser(): _currentState(METHOD_PARSING){}
RequestLineParser::RequestLineParser(const RequestLineParser& other){}
RequestLineParser&  RequestLineParser::operator=(const RequestLineParser& other){}
RequestLineParser::~RequestLineParser(){}

void    RequestLineParser::updateAndParse(std::string& extraBuffer)
{
    _currentBuffer += extraBuffer;
    
    switch (_currentState)
	{
		case METHOD_PARSING:
			_parseMethod();
		case TARGET_PARSING:
			// TODO: continue target parsing
		case VERSION_PARSING:
			// TODO: continue version parsing
		default:
			std::cout << "LOLADA MIX" << std::endl;
	}
}

void    RequestLineParser::_parseMethod(void)
{
    char			buff[7] = {};
	unsigned int	size = 0;

	while (_currentBuffer[size] != ' ' && size < MAX_METHOD_SIZE + 1)
		size++;
	std::string	buffStr(buff, size);
	for (int i=0; i<METHODS_AM; i++)
	{
		if (buffStr == _methods[i])
			_method = static_cast<Method>(i);
	}
	if (_method == INVALID)
		std::cerr << "TREAT INVALID METHOD PLZZZZZZZZZ" << std::endl; //TODO
    _currentState = TARGET_PARSING;
    _currentBuffer = _currentBuffer.substr(size + 1);
    //TODO test this func
}

void    RequestLineParser::_parseTarget(void)
{
    size_t  sPos = _currentBuffer.find(' ');
}

void    RequestLineParser::_parseVersion(void)
{
    //TODO
}