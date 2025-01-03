/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:30:33 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/01/03 19:18:36 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConfigFile.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::ConfigFile(int argc, char **argv)
{
	/* if (argc != 2)
		createFile(); */
	(void)argc;
	_filePath = argv[1];
	run();
}

ConfigFile::ConfigFile(const ConfigFile& src)
{
	_filePath = src._filePath;
}

ConfigFile& ConfigFile::operator=(const ConfigFile& src)
{
	if (this != &src)
		_filePath = src._filePath;
	return (*this);
}

ConfigFile::~ConfigFile(){}

void ConfigFile::run()
{
	try
	{
		this->isConfigFilePath();
		this->readingFile();
		this->splitServers();
	}
	catch(const std::exception& e)
	{
		std::cerr << "[ERROR]: " << e.what() << std::endl;
	}
}

const std::string& ConfigFile::getContent() const
{
	return (_content);
} 

size_t ConfigFile::getNbrOfServers() const
{
	return (_nbrOfServers);
}

Server ConfigFile::getServer(size_t indexNbr) const
{
	return (_serverObjs[indexNbr]);
}

void ConfigFile::isConfigFilePath()
{
	struct stat statbuf;
	
	if (stat(_filePath.c_str(), &statbuf) == 0)
	{
		if (S_ISREG(statbuf.st_mode) && statbuf.st_mode & S_IRUSR)
			return ;
	}
	throw std::invalid_argument("Configuration File: Invalid path.");
}

void ConfigFile::readingFile()
{
	std::ifstream inFile;
	size_t auxPos = 0;
	std::string auxStr;

	inFile.open(_filePath.c_str(), std::ios::out);
	if (!inFile.good())
		throw std::runtime_error("Can't open configuration file.");
	
	std::string line;
	while (getline(inFile, line))
	{
		if ((auxPos = line.find("#")) != std::string::npos)
			line = line.substr(0, auxPos - 1);
		if (line.length() == 0 || std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))) == line.end())
            continue;
		for (size_t i = 0; i < line.size(); ++i)
		{
			if (!isspace(line[i]))
			{
				auxPos = i;
				break ;
			}
		}
		line = line.substr(auxPos);
		_content = _content + line + "\n";
	}
	
	if (_content.empty() || _content.length() == 0)
		throw std::runtime_error("Configuration File is empty.");
	for (size_t i = 0; i <= _content.length(); i++)
	{
		if (!std::isspace(_content[i]))
			break ;
		if (i == _content.length() - 1)
			throw std::runtime_error("Configuration File is empty.");
	}
}

void ConfigFile::splitServers()
{
	if (std::count(_content.begin(), _content.end(), '{') != std::count(_content.begin(), _content.end(), '}'))
		throw std::runtime_error("The number of opening curly brace is different of the number of closing curly brace.");
	
	_serverStr = splitServerBlocks(_content);
	_nbrOfServers = _serverStr.size();
	
	for (size_t i = 0; i < _serverStr.size(); ++i) 
	{
		_serverObjs.push_back(fillServersObjs(_serverStr[i], i + 1));
	}
	
	//std::cout << _serverObjs[0].getLocation()[0].getRoot() << std::endl;
	//std::cout << _serverObjs[0].getPort(0) << '\n';
	//std::cout << _serverObjs[0].getServerName() << '\n';
	// std::cout << _serverObjs[0].getHost() << '\n';
	// std::cout << _serverObjs[0].getClientLimit() << '\n';
	// std::cout << _serverObjs[0].getRoot() << '\n';
	// std::cout << _serverObjs[0].getIndex(1) << '\n';
	// std::cout << _serverObjs[0].getErrorPage("401") << '\n';
}

Server ConfigFile::fillServersObjs(std::string& serverStr, size_t serverId)
{
	std::vector<std::string> serverVector = splitStr(serverStr, '\n');
	Server realServer;

	realServer.setServerId(serverId);
	for (size_t i = 0; i < serverVector.size(); ++i) 
	{
		if (!serverVector[i].compare("}"))
			continue ;
		//std::cout << serverVector[i] << std::endl;
		if (serverVector[i].find("location") != std::string::npos)
		{
			realServer.setLocation(serverVector, i);
			// std::cout << i << std::endl;
			for (size_t j = i; j < serverVector.size() - 1; j++)
			{
				if (serverVector[j].find("}") != std::string::npos)
					i = j;
			}
		}
		else if (i != 0)
			realServer.setElements(serverVector[i]);		
	}
	return (realServer);
}
