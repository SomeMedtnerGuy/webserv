/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:09:54 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/02/16 21:09:21 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
# include <netinet/in.h>

enum Method {
	GET,
	POST,
	DELETE,
	METHOD_SIZE,
	UNKNOWN
};

void checkLine(std::string& line, int lineNbr);

std::vector<std::string> splitServerBlocks(const std::string& content);
std::vector<std::string> splitStr(const std::string& Str, char delimiter);
std::vector<std::string> splitServerStr(const std::string& Str);
std::string intToStr(int i);
bool validDomain(std::string& domainToValidate);
void checkSemicolonAtEnd(const std::string& str, int _serverId, std::string directive);
void isValidIPv4(std::vector<std::string>& str);
bool isDirectory(std::string path);
bool isFile(std::string path);
void checkCurlyBrace(std::string& line, bool begin);
std::string cacthPath(std::string str);
Method	strToMethod(std::string method);
int	getPortFromSocket(int sockfd);
size_t getFileLength(std::string filename);

template<typename T>
static std::string  ntostr(T number)
{
	std::stringstream   converter;
	converter << number;
    return (converter.str());
}

#endif
