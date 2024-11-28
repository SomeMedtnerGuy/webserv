// C++ program to show the example of server application in
// socket programming
// #include <cstring>
// #include <iostream>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <unistd.h>

// using namespace std;

// int main()
// {
//     int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 	if (serverSocket < 0) {
//         return -1;
//     }

//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(8080);
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
//         perror("Erro ao associar socket");
//         close(serverSocket);
//         return -1;
//     }

//     if (listen(serverSocket, 5) < 0) {
//         perror("Erro ao escutar");
//         close(serverSocket);
//         return -1;
//     }

// 	std::cout << "Servidor rodando em http://localhost:8080\n";

//    while (true) {
//         int clientSocket = accept(serverSocket, nullptr, nullptr);
//         if (clientSocket < 0) {
//             perror("Erro ao aceitar conexão");
//             continue;
//         }

//         char buffer[1024] = {0};
//         recv(clientSocket, buffer, sizeof(buffer), 0);

//         std::cout << "Requisição recebida:\n" << buffer << "\n";

//         const char* response =
//             "HTTP/1.1 200 OK\r\n"
//             "Content-Type: text/html\r\n"
//             "Content-Length: 49\r\n"
//             "\r\n"
//             "<html><body><h1>Hello, world!</h1></body></html>";

//         send(clientSocket, response, strlen(response), 0);

//         close(clientSocket);
//     }

//     close(serverSocket);
//     return 0;
// }

#include "../include/ConfigFile.hpp"

int main(int argc, char **argv)
{

	if (argc != 2)
	{
		std::cout << "Error: Provide a path to the configuration file as a program argument." << std::endl;
		return (1);
	}
	ConfigFile checkPath(argv[1]);
	//ConfigParameters checkParameters;
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

}


