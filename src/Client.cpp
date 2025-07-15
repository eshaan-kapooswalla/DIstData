#include <iostream>
#include <string>
#include <vector>
#include "Communication.h"
#include "message.h"
#include "message_serializer.h"
#include "message_deserializer.h"

int main() {
    std::cout << "Client started. Connecting to CoordinatorNode..." << std::endl;
    // Connect to CoordinatorNode (server)
    int sock = Communication::startClient("127.0.0.1", 8080);
    if (sock < 0) {
        std::cerr << "Failed to connect to CoordinatorNode." << std::endl;
        return 1;
    }

    // Prepare a DATA_REQUEST message
    Message msg;
    msg.type = MessageType::DATA_REQUEST;
    msg.key_value.key = "users";
    msg.key_value.value = ""; // No value for request
    std::vector<uint8_t> serialized = MessageSerializer::serialize(msg);

    // Send the serialized message
    std::string out(reinterpret_cast<const char*>(serialized.data()), serialized.size());
    if (!Communication::sendMessage(sock, out)) {
        std::cerr << "Failed to send message." << std::endl;
        Communication::closeSocket(sock);
        return 1;
    }

    // Receive the response
    std::string response = Communication::receiveMessage(sock);
    Communication::closeSocket(sock);
    if (response.empty()) {
        std::cerr << "No response from CoordinatorNode." << std::endl;
        return 1;
    }
    std::vector<uint8_t> respBuf(response.begin(), response.end());
    Message respMsg = MessageDeserializer::deserialize(respBuf);
    if (respMsg.type == MessageType::DATA_RESPONSE) {
        std::cout << "Client received data: key='" << respMsg.key_value.key << "', value='" << respMsg.key_value.value << "'" << std::endl;
    } else {
        std::cout << "Client received unknown response type." << std::endl;
    }
    return 0;
} 