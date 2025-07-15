#ifndef MESSAGE_DESERIALIZER_H
#define MESSAGE_DESERIALIZER_H

#include "message.h"
#include <vector>
#include <cstdint>

class MessageDeserializer {
public:
    static Message deserialize(const std::vector<uint8_t>& buffer);
};

#endif // MESSAGE_DESERIALIZER_H 