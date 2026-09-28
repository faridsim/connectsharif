
#pragma once

#include "MessageType.h"
#include <string>
//each message has a type,id ,and payload
//The final keyword prevents other classes from inheriting from Message
class Message final
{
public:
    explicit Message(int id, MessageType type, std::string payload)
    //s-em
        : id(id)

        , type(type)
        , payload(payload)
    {
    }

    int id;
    MessageType type;
    std::string payload;
};


