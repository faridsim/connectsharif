#pragma once

#include "MessageType.h"
#include <string>

class Message final
{
public:
    explicit Message(int id, MessageType type, std::string payload)
        : id(id)
        , type(type)
        , payload(payload)
    {
    }

    int id;
    MessageType type;
    std::string payload;
};