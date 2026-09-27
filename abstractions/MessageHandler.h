#pragma once

#include "../models/Message.h"

class MessageHandler
{
public:
    MessageHandler() = default;
    MessageHandler(const MessageHandler&) = delete;
    MessageHandler(MessageHandler&&) = delete;
    MessageHandler& operator=(const MessageHandler&) = delete;
    MessageHandler& operator=(MessageHandler&&) = delete;

    virtual ~MessageHandler() = default;

    virtual bool CanHandle(MessageType type) const = 0;
    virtual void Process(Message message) = 0;
};
