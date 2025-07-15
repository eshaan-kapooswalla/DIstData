#include "Communication.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int Communication::startServer(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return server_fd;
}

int Communication::startClient(const std::string& host, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sock);
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        close(sock);
        return -1;
    }
    return sock;
}

bool Communication::sendMessage(int socket, const std::string& message) {
    ssize_t sent = send(socket, message.c_str(), message.size(), 0);
    return sent == (ssize_t)message.size();
}

std::string Communication::receiveMessage(int socket) {
    char buffer[1024] = {0};
    ssize_t valread = read(socket, buffer, 1024);
    if (valread > 0) {
        return std::string(buffer, valread);
    }
    return std::string();
}

void Communication::closeSocket(int socket) {
    close(socket);
} 