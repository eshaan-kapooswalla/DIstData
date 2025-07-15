#pragma once
#include <string>

class Communication {
public:
    // Start a server socket listening on the given port
    static int startServer(int port);

    // Connect to a server at host:port
    static int startClient(const std::string& host, int port);

    // Send a message over a socket
    static bool sendMessage(int socket, const std::string& message);

    // Receive a message from a socket
    static std::string receiveMessage(int socket);

    // Close a socket
    static void closeSocket(int socket);
}; 