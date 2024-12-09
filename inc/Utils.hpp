/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 22:25:57 by joamonte          #+#    #+#             */
/*   Updated: 2024/12/08 23:04:31 by joamonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Define.hpp"

// ---> Boolean checkers -----------------------------------------------------

bool			isValidKeyword(std::string const &s);

// ---> String Utils --------------------------------------------------------

std::string					intToString(int n);
std::vector<std::string>	getStringTokens(std::string const &s, char c);
void						stringTrim(std::string &s, char const *set);
std::string					getPathWithSlashAtEnd(std::string const &path);
std::string					getPathWithoutSlashAtEnd(std::string const &path);
std::string					getPathWithoutSlashAtBegin(std::string const &path);
std::string					getCorrectPath(std::string const &root, std::string const &resource);
bool						isItSufix(std::string const &s, std::string const &sufix);
std::string					convertScriptNameToTitle(std::string const &scriptName);
std::string					getPathWithoutFilename(std::string const &path);
std::string					urlDecode(const std::string &input);


// ---> Time Utils ----------------------------------------------------------

std::string		getTime();
std::string		getTimeStamp();

// ---> File Utils ----------------------------------------------------------

bool			isReadbleFile(std::string const &path);
bool			hasThisExtension(std::string const &file, std::string const &extension);
bool			directoryExists(std::string const &path);
std::string		getFileContent(std::string const &path);
std::string		getFileType(std::string const &file);
std::string		generateResponseOK(std::string const &path);
std::string		generateResponseWithCustomHTML(std::string const &code, std::string const &title, std::string const &body);

// ---> Display Utils -----------------------------------------------------------

void			printMsg(std::string const &s, std::string const &color);

// ---> Signal Utils -----------------------------------------------------------

void			signalHandler(int signum);



#endif