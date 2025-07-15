#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include "Communication.h"

void logMessage(const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::cout << std::ctime(&now) << ": " << message << std::endl;
}

int main() {
    int sock = Communication::startClient("127.0.0.1", 8081);
    if (sock < 0) {
        logMessage("Failed to connect to server.");
        return 1;
    }
    for (int i = 0; i < 5; ++i) {
        std::string message = "Message " + std::to_string(i) + " from client";
        if (!Communication::sendMessage(sock, message)) {
            logMessage("Failed to send message: " + message);
            continue;
        }
        logMessage("Message sent: " + message);
        std::string buffer = Communication::receiveMessage(sock);
        logMessage("Response from server: " + buffer);
        sleep(1); // Wait a bit between messages
    }
    Communication::closeSocket(sock);
    return 0;
} 