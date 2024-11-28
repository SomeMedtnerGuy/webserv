// C++ program to show the example of server application in
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <exception>

using namespace std;

int main()
{

    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket < 0) {
        return -1;
    }

    try{
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(1023);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            std::cout << "NICOLE E FEIA :P";
            close(serverSocket);
            return -1;
        }

        if (listen(serverSocket, 5) < 0) {
            close(serverSocket);
            return -1;
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
    }

	std::cout << "server on http://localhost:8080\n";

   while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            continue;
        }

        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);

        std::cout << "Requisição recebida:\n" << buffer << "\n";

        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 49\r\n"
            "\r\n"
            "<html><body><h1>Hello, world!</h1></body></html>";

        send(clientSocket, response, strlen(response), 0);

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
