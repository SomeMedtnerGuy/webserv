/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:28:15 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/11/27 18:31:07 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "utils.hpp"
#include "Server.hpp"

class ConfigFile {
private:
	std::string _filePath;
	std::string _content;
	size_t _nbrOfServers;
	std::vector<std::string> _serverStr;
	std::vector<Server> _serverObjs;

public:
	ConfigFile();
	ConfigFile(std::string filePath);
	ConfigFile(const ConfigFile& src);
	ConfigFile& operator=(const ConfigFile& src);
	~ConfigFile();

	const std::string& getContent() const;

	void isConfigFilePath();
	void readingFile();
	void splitServers();
	Server fillServersObjs(std::string& serverStr);

};

#endif // CONFIGFILE_HPP