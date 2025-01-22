/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:28:15 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/01/22 17:37:31 by ndo-vale         ###   ########.fr       */
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
	ConfigFile();

public:
	ConfigFile(int argc, char **argv);
	ConfigFile(const ConfigFile& src);
	ConfigFile& operator=(const ConfigFile& src);
	~ConfigFile();

	void run();
	void createFile();
	const std::string& getContent() const;
	size_t getNbrOfServers() const;
	Server getServer(std::string serverName) const;
	void clearDuplicateServers();
	/* check more than one server name  */

	void isConfigFilePath();
	void readingFile();
	void splitServers();
	Server fillServersObjs(std::string& serverStr, size_t serverId);

};

#endif // CONFIGFILE_HPP