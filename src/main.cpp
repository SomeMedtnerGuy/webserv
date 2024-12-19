#include "../include/ConfigFile.hpp"

int main(int argc, char **argv)
{
	ConfigFile parsing(argc, argv);
	parsing.run();
	std::string location = "/";
	parsing.getServer(0).getLocation(location);
	// std::cout << parsing.getServer(0).getHost() << std::endl;
	// std::cout << parsing.getServer(0).getIndex(0) << std::endl;
/* 
	if (argc != 2)
	{
		std::cout << "Error: Provide a path to the configuration file as a program argument." << std::endl;
		return (1);
	}
	ConfigFile checkPath(argv[1]);
	try
	{
		checkPath.isConfigFilePath();
		checkPath.readingFile();
		checkPath.splitServers();
	}
	catch(const std::exception& e)
	{
		std::cerr << "[ERROR]: " << e.what() << std::endl;
	}
 */
}


