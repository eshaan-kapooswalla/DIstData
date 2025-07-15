#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <cstdint>

enum class MessageType : uint8_t {
    UNKNOWN = 0,
    NODE_REGISTRATION = 1,
    DATA_REQUEST = 2,
    DATA_RESPONSE = 3,
    NODE_LIST_REQUEST = 4,
    NODE_LIST_RESPONSE = 5,
};

struct RegistrationData {
    uint32_t node_id;
    std::string node_address;
};

struct KeyValueData {
    std::string key;
    std::string value;
};

struct NodeInfo {
    std::string uuid;
    std::string ip;
    int port;
};

struct NodeListData {
    std::vector<NodeInfo> nodes;
};

struct Message {
    MessageType type;
    RegistrationData registration; // Used for NODE_REGISTRATION
    KeyValueData key_value;        // Used for DATA_REQUEST and DATA_RESPONSE
    NodeInfo node_info;            // Used for NODE_REGISTRATION and NODE_LIST
    NodeListData node_list;        // Used for NODE_LIST_RESPONSE
};

#endif // MESSAGE_H 