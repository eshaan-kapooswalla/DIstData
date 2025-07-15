#ifndef MESSAGE_SERIALIZER_H
#define MESSAGE_SERIALIZER_H

#include "message.h"
#include <vector>
#include <cstdint>

class MessageSerializer {
public:
    static std::vector<uint8_t> serialize(const Message& message);
};

#endif // MESSAGE_SERIALIZER_H 