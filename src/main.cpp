#include "../include/ConfigFile.hpp"
#include "../include/ServerSettings.hpp"

int main(int argc, char **argv)
{
	ConfigFile parsing(argc, argv);
	//std::cout << parsing.getServer("localhost").getLocation()[0].getSpecificPath() << std::endl;
	// std::cout << "test" << std::endl;

	//std::cout << parsing.getServer("localhosta").getServerName() << std::endl;

	ServerSettings serversettings(parsing);
	serversettings.setServer("localhost");
	std::cout << serversettings.getIndex() << std::endl;

	serversettings.setLocation("/");
	std::cout << serversettings.getIndex() << std::endl;


	/* check if root is a directory in location */
	// std::ifstream file(serversettings.getErrorPage(404).c_str());
	// if (file.is_open())
		//std::cout << serversettings.getErrorPage(404) << std::endl;
	// std::cout << serversettings.getReturnURL() << std::endl;
	
	// parsing.run();
	// std::string location = "/";
	// parsing.getServer(0).getLocation();
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


