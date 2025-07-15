#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include "Communication.h"
#include "message.h"
#include "message_serializer.h"
#include "message_deserializer.h"

// Simple UUID generator for demo
std::string generateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::stringstream ss;
    for (int i = 0; i < 32; ++i) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

int main() {
    std::string myUUID = generateUUID();
    std::string myIP = "127.0.0.1";
    int myPort = 9000; // Example port for this node
    std::cout << "DataNode started. UUID=" << myUUID << ", IP=" << myIP << ", Port=" << myPort << std::endl;

    // Register with CoordinatorNode (Registrar)
    int sock = Communication::startClient("127.0.0.1", 8080);
    if (sock < 0) {
        std::cerr << "Failed to connect to CoordinatorNode for registration." << std::endl;
        return 1;
    }
    Message regMsg;
    regMsg.type = MessageType::NODE_REGISTRATION;
    regMsg.node_info.uuid = myUUID;
    regMsg.node_info.ip = myIP;
    regMsg.node_info.port = myPort;
    std::vector<uint8_t> serialized = MessageSerializer::serialize(regMsg);
    std::string out(reinterpret_cast<const char*>(serialized.data()), serialized.size());
    Communication::sendMessage(sock, out);
    Communication::closeSocket(sock);

    // Discover nodes (request node list)
    int sock2 = Communication::startClient("127.0.0.1", 8080);
    if (sock2 < 0) {
        std::cerr << "Failed to connect to CoordinatorNode for discovery." << std::endl;
        return 1;
    }
    Message reqMsg;
    reqMsg.type = MessageType::NODE_LIST_REQUEST;
    std::vector<uint8_t> serializedReq = MessageSerializer::serialize(reqMsg);
    std::string outReq(reinterpret_cast<const char*>(serializedReq.data()), serializedReq.size());
    Communication::sendMessage(sock2, outReq);
    std::string response = Communication::receiveMessage(sock2);
    Communication::closeSocket(sock2);
    if (response.empty()) {
        std::cerr << "No response from CoordinatorNode for node list." << std::endl;
        return 1;
    }
    std::vector<uint8_t> respBuf(response.begin(), response.end());
    Message respMsg = MessageDeserializer::deserialize(respBuf);
    if (respMsg.type == MessageType::NODE_LIST_RESPONSE) {
        std::cout << "Discovered nodes:" << std::endl;
        for (const auto& node : respMsg.node_list.nodes) {
            std::cout << "  UUID=" << node.uuid << ", IP=" << node.ip << ", Port=" << node.port << std::endl;
        }
    } else {
        std::cout << "Unexpected response type from CoordinatorNode." << std::endl;
    }
    return 0;
} 