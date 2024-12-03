/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:11:45 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/12/03 16:42:25 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"

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

bool validDomain(std::string& domainToValidate)
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
