/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:11:45 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/02/16 15:02:38 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/utils.hpp"

std::vector<std::string> splitServerBlocks(const std::string& content)
{
    std::vector<std::string> serverBlocks;
    size_t pos = 0;
    size_t openBracket = 0;
    size_t closeBracket = 0;
    int balance = 0;

    while (pos < content.size())
	{
        openBracket = content.find("server", pos);
        if (openBracket == std::string::npos)
            break;
        openBracket += 6;
        while (isspace(content[openBracket]))
            openBracket++;
        if (content[openBracket] != '{')
            throw std::runtime_error("aaaa");

        balance = 0;
        size_t start = openBracket;
        size_t end = openBracket;

        while (end < content.size())
		{
            if (content[end] == '{') 
			{
				if (content[end + 1] != '\n')
					throw std::runtime_error("Opening brace '{' should be at the end of the line, without spaces.");
                balance++;
            } 
			else if (content[end] == '}') 
			{
                if (content[end + 1] != '\n' || content[end - 1] != '\n')
					throw std::runtime_error("Closing brace '}' should be on a separate line, without spaces at the end.");
				balance--;
                if (balance == 0)
				{
                    closeBracket = end;
                    break;
                }
            }
            end++;
        }

        if (balance != 0)
            throw std::runtime_error("Mismatched braces in server blocks.");

        serverBlocks.push_back("server " + content.substr(start, closeBracket - start + 1));
        pos = closeBracket + 1;
    }

    return serverBlocks;
}

std::vector<std::string> splitStr(const std::string& Str, char delimiter)
{
    std::vector<std::string> StrVector;
    
    size_t start = 0;
    size_t end = 0;
    while (start < Str.size())
    {
        end = Str.find(delimiter, start);
        
        if (end == std::string::npos)
        {
            StrVector.push_back(Str.substr(start));
            break ;
        }
        
        StrVector.push_back(Str.substr(start, end - start));
        start = end + 1;
    }
    return (StrVector);
}

std::vector<std::string> splitServerStr(const std::string& serverStr)
{
    std::vector<std::string> serverStrVector;
    
    size_t start = 0;
    size_t end = 0;
    while (start < serverStr.size())
    {
        while (start < serverStr.size() && isspace(serverStr[start]))
            start++;
        if (start >= serverStr.size())
            break;
        
        end = start;
        while (end < serverStr.size() && !isspace(serverStr[end]))
            end++;
        
        serverStrVector.push_back(serverStr.substr(start, end - start));
        start = end + 1;
    }
    return serverStrVector;
}

std::string intToStr(int i)
{
    std::stringstream ss;
    ss << i;
    return (ss.str());
}

bool validDomain(std::string& domainToValidate)/* check this */
{
    if (domainToValidate.find('-') == 0)
        throw std::runtime_error("Invalid server_name value.");
    return (true);      
}

void checkSemicolonAtEnd(const std::string& str, int _serverId, std::string directive)
{
    if (str[str.size() - 1] != ';')
        throw std::runtime_error("The " + directive + " directive in server " + intToStr(_serverId) + " block does not end with \';\'");
}

void isValidIPv4(std::vector<std::string>& str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        for (size_t j = 0; j < str[i].size(); j++)
        {
            if (!isdigit(str[i][j]))
                throw std::runtime_error("Invalid host directive: Expected an IPv4 address in the format 'x.x.x.x' with numbers values.");
        }
        if (atoi(str[i].c_str()) < 0 || atoi(str[i].c_str()) > 255)
            throw std::runtime_error("Invalid host directive: Expected an IPv4 address in the format 'x.x.x.x' with values between 0 and 255.");
    }
}

bool isDirectory(std::string path)
{
    struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0 || !(statbuf.st_mode & S_IFDIR))
    {
        perror("Erro ao acessar o path");
        return (false);
    }
    return (true);
}

bool isFile(std::string path)
{
    struct stat statbuf;
	if (stat(path.c_str(), &statbuf) == 0)
	{
		if (S_ISREG(statbuf.st_mode) && statbuf.st_mode & S_IRUSR)
			return (true);
	}
    return (false);
}

void checkCurlyBrace(std::string& line, bool begin)
{
    if (begin)
    {
        if (line.find("{") == std::string::npos || line.find("{") != line.size() - 1)
            throw std::runtime_error("The location block must be open in the same line, with a opening curly brace '{' and it should be at the end of the line, without spaces.");    
    }
    else
    {
        if (line.compare("}"))
            throw std::runtime_error("The location block must be ended with a closing curly brace, in a diferent line.");
    }
}

std::string cacthPath(std::string str)
{
    std::string path = str.erase(str.size() - 1);
    std::vector<std::string> pathVec = splitStr(path, ' ');
    if (pathVec.size() != 2)
        throw std::runtime_error("The location block must have only one specific path.");
 
    return (pathVec[1]);
}

Method  strToMethod(std::string str)
{
    const std::string methods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++)
	{
		if (str.compare(0, methods[i].length() + 1, methods[i] + " ") == 0)
			return (static_cast<Method>(i));
	}
	return (UNKNOWN);
}

void eraseExtraSpaces(std::string& line)
{
    std::istringstream stream(line);
    std::string word, result;

    while (stream >> word) {
        if (!result.empty())
            result += ' ';
        result += word;
    }

    line.clear();
    line = result;
}

void eraseComents(std::string& line)
{
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '#')
        {
            line = line.substr(0, i);
            return ;
        }
    }
}

void checkSemicolon(std::string& line, int lineNbr)
{
    int end = line.size() - 1;
    
    if (line.size() == 0)
        return;
    
    if (line[end] == '{' || line[end] == '}' || line[end] == ';')
    {
        if (line[end] == ';')
        {
            if (std::isspace(line[end - 1]))
                throw std::runtime_error("Line " + intToStr(lineNbr) + " contains forbidden space before the semicolon.");                   
            line.erase(end);        
        }
        return ;
    }
    throw std::runtime_error("Line " + intToStr(lineNbr) + " ends with an invalid character. The lines must end with ;, { or }");
}

void checkLine(std::string& line, int lineNbr)
{
    eraseComents(line);
    eraseExtraSpaces(line);
    checkSemicolon(line, lineNbr);
}

int		getPortFromSocket(int sockfd)
{
	struct sockaddr_in	addr;
	socklen_t	len = sizeof(addr);
	getsockname(sockfd, (struct sockaddr*)&addr, &len);
	return (ntohs(addr.sin_port));
}

size_t getFileLength(std::string filename)
{
    std::ifstream       file(filename.c_str()); //TODO: Protect this
    std::streampos      length;
    std::streampos      pos(file.tellg());
    
    file.seekg(pos, std::ios::end);
    length = file.tellg();
    return (static_cast<int>(length));
}

// if ((auxPos = line.find("#")) != std::string::npos)
		// 	line = line.substr(0, auxPos - 1);
		// if (line.length() == 0 || std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))) == line.end())
        //     continue;
		// for (size_t i = 0; i < line.size(); ++i)
		// {
		// 	if (!isspace(line[i]))
		// 	{
		// 		auxPos = i;
		// 		break ;
		// 	}
		// }
