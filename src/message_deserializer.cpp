#include "message_deserializer.h"
#include <cstring>
#include <stdexcept>

static uint32_t readUint32(const std::vector<uint8_t>& buffer, size_t& pos) {
    if (pos + 4 > buffer.size()) throw std::runtime_error("Buffer underflow");
    uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        value = (value << 8) | buffer[pos++];
    }
    return value;
}

static int readInt(const std::vector<uint8_t>& buffer, size_t& pos) {
    if (pos + 4 > buffer.size()) throw std::runtime_error("Buffer underflow");
    int value = 0;
    for (int i = 0; i < 4; ++i) {
        value = (value << 8) | buffer[pos++];
    }
    return value;
}

static std::string readString(const std::vector<uint8_t>& buffer, size_t& pos) {
    uint32_t len = readUint32(buffer, pos);
    if (pos + len > buffer.size()) throw std::runtime_error("Buffer underflow");
    std::string str(buffer.begin() + pos, buffer.begin() + pos + len);
    pos += len;
    return str;
}

static NodeInfo readNodeInfo(const std::vector<uint8_t>& buffer, size_t& pos) {
    NodeInfo node;
    node.uuid = readString(buffer, pos);
    node.ip = readString(buffer, pos);
    node.port = readInt(buffer, pos);
    return node;
}

Message MessageDeserializer::deserialize(const std::vector<uint8_t>& buffer) {
    Message message;
    size_t pos = 0;
    if (buffer.empty()) throw std::runtime_error("Empty buffer");
    message.type = static_cast<MessageType>(buffer[pos++]);
    switch (message.type) {
        case MessageType::NODE_REGISTRATION:
            message.node_info = readNodeInfo(buffer, pos);
            break;
        case MessageType::NODE_LIST_REQUEST:
            // No payload
            break;
        case MessageType::NODE_LIST_RESPONSE: {
            uint32_t count = readUint32(buffer, pos);
            for (uint32_t i = 0; i < count; ++i) {
                message.node_list.nodes.push_back(readNodeInfo(buffer, pos));
            }
            break;
        }
        case MessageType::DATA_REQUEST:
        case MessageType::DATA_RESPONSE:
            message.key_value.key = readString(buffer, pos);
            message.key_value.value = readString(buffer, pos);
            break;
        default:
            // Unknown type: do nothing or throw
            break;
    }
    return message;
} 