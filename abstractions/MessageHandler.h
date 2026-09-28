#pragma once

#include "../models/Message.h"

class MessageHandler
{
public:
    explicit MessageHandler() = default;
    //copy constructor
    MessageHandler(const MessageHandler&) = delete;
    //move assigment operator
    MessageHandler(MessageHandler&&) = delete;
    //delete copy assigment operator
    MessageHandler& operator=(const MessageHandler&) = delete;
    //move assgiment operator
    MessageHandler& operator=(MessageHandler&&) = delete;
    //destructor is defualt
    virtual ~MessageHandler() = default;
    //make it const so it cant change the message type
    virtual bool CanHandle(MessageType type) const = 0;
    //?? no virtual dispatch
    virtual void Process(Message message) = 0;
};
