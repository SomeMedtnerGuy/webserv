#include "../include/ConfigFile.hpp"
#include "../include/ServerSettings.hpp"

int main(int argc, char **argv)
{
	ConfigFile parsing(argc, argv);

	//std::cout << parsing.getServer("hostname").getLocation()[0].getAllowMethods("GET") << std::endl;

	std::cout << "Server Settings default:" << std::endl;

	ServerSettings serversettings(parsing);
	std::cout << "Server name " << serversettings.getServerName() << std::endl;
	std::cout << "Root " << serversettings.getRoot() << std::endl;
	std::cout << "Index " << serversettings.getIndex() << std::endl;
	std::cout << "Error page 404 " << serversettings.getErrorPage(404) << std::endl;
	std::cout << "Return code " << serversettings.getReturnCode() << std::endl;
	std::cout << "Return URL " << serversettings.getReturnURL() << std::endl;
	std::cout << "Client body size " << serversettings.getClientBodySize() << std::endl;
	std::cout << "Auto index " << serversettings.getAutoIndex() << std::endl;
	std::cout << "GET " << serversettings.getAllowMethod(GET) << std::endl;
	std::cout << "POST " << serversettings.getAllowMethod(POST) << std::endl;
	std::cout << "DELETE " << serversettings.getAllowMethod(DELETE) << std::endl;
	std::cout << "UNKNOWN " << serversettings.getAllowMethod(UNKNOWN) << std::endl;

	std::cout << std::endl;
	std::cout << "Set server localhost:" << std::endl;
	serversettings.setServer("localhost");

	std::cout << "Server name " << serversettings.getServerName() << std::endl;
	std::cout << "Root " << serversettings.getRoot() << std::endl;
	std::cout << "Index " << serversettings.getIndex() << std::endl;
	std::cout << "Error page 404 " << serversettings.getErrorPage(404) << std::endl;
	std::cout << "Return code " << serversettings.getReturnCode() << std::endl;
	std::cout << "Return URL " << serversettings.getReturnURL() << std::endl;
	std::cout << "Client body size " << serversettings.getClientBodySize() << std::endl;
	std::cout << "Auto index " << serversettings.getAutoIndex() << std::endl;
	std::cout << "GET " << serversettings.getAllowMethod(GET) << std::endl;
	std::cout << "POST " << serversettings.getAllowMethod(POST) << std::endl;
	std::cout << "DELETE " << serversettings.getAllowMethod(DELETE) << std::endl;
	std::cout << "UNKNOWN " << serversettings.getAllowMethod(UNKNOWN) << std::endl;

	std::cout << std::endl;
	std::cout << "Set location /:" << std::endl;
	serversettings.setLocation("/");

	std::cout << "Server name " << serversettings.getServerName() << std::endl;
	std::cout << "Root " << serversettings.getRoot() << std::endl;
	std::cout << "Index " << serversettings.getIndex() << std::endl;
	std::cout << "Error page 404 " << serversettings.getErrorPage(404) << std::endl;
	std::cout << "Return code " << serversettings.getReturnCode() << std::endl;
	std::cout << "Return URL " << serversettings.getReturnURL() << std::endl;
	std::cout << "Client body size " << serversettings.getClientBodySize() << std::endl;
	std::cout << "Auto index " << serversettings.getAutoIndex() << std::endl;
	std::cout << "GET " << serversettings.getAllowMethod(GET) << std::endl;
	std::cout << "POST " << serversettings.getAllowMethod(POST) << std::endl;
	std::cout << "DELETE " << serversettings.getAllowMethod(DELETE) << std::endl;
	std::cout << "UNKNOWN " << serversettings.getAllowMethod(UNKNOWN) << std::endl;

}


