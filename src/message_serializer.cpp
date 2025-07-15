#include "message_serializer.h"
#include <cstring>

static void writeUint32(std::vector<uint8_t>& buffer, uint32_t value) {
    for (int i = 0; i < 4; ++i)
        buffer.push_back((value >> (24 - i * 8)) & 0xFF);
}

static void writeInt(std::vector<uint8_t>& buffer, int value) {
    for (int i = 0; i < 4; ++i)
        buffer.push_back((value >> (24 - i * 8)) & 0xFF);
}

static void writeString(std::vector<uint8_t>& buffer, const std::string& str) {
    writeUint32(buffer, static_cast<uint32_t>(str.size()));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

static void writeNodeInfo(std::vector<uint8_t>& buffer, const NodeInfo& node) {
    writeString(buffer, node.uuid);
    writeString(buffer, node.ip);
    writeInt(buffer, node.port);
}

std::vector<uint8_t> MessageSerializer::serialize(const Message& message) {
    std::vector<uint8_t> buffer;
    buffer.push_back(static_cast<uint8_t>(message.type));
    switch (message.type) {
        case MessageType::NODE_REGISTRATION:
            writeNodeInfo(buffer, message.node_info);
            break;
        case MessageType::NODE_LIST_REQUEST:
            // No payload needed
            break;
        case MessageType::NODE_LIST_RESPONSE:
            writeUint32(buffer, static_cast<uint32_t>(message.node_list.nodes.size()));
            for (const auto& node : message.node_list.nodes) {
                writeNodeInfo(buffer, node);
            }
            break;
        case MessageType::DATA_REQUEST:
        case MessageType::DATA_RESPONSE:
            writeString(buffer, message.key_value.key);
            writeString(buffer, message.key_value.value);
            break;
        default:
            // Unknown type: do nothing or throw
            break;
    }
    return buffer;
} 