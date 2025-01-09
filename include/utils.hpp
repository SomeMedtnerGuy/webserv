/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:09:54 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/01/09 14:03:02 by nsouza-o         ###   ########.fr       */
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

enum Method {
	GET,
	POST,
	DELETE,
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

#endif