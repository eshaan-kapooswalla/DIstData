#include <iostream>
#include <string>
#include <ctime>
#include "Communication.h"

void logMessage(const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::cout << std::ctime(&now) << ": " << message << std::endl;
}

int main() {
    int server_fd = Communication::startServer(8081);
    if (server_fd < 0) {
        logMessage("Failed to start server.");
        return 1;
    }
    logMessage("TestServer listening on port 8081...");
    while (true) {
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        int client_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_sock < 0) {
            logMessage("accept failed");
            continue;
        }
        logMessage("Connection accepted from client");
        std::string buffer = Communication::receiveMessage(client_sock);
        logMessage("Message from client: " + buffer);
        std::string response = "Hello from server";
        Communication::sendMessage(client_sock, response);
        logMessage("Response sent");
        Communication::closeSocket(client_sock);
    }
    Communication::closeSocket(server_fd);
    return 0;
} 