#pragma once

#include "../../abstractions/MessageHandler.h"

#include <iostream>

class PushMessageHandler final : public MessageHandler
{
public:
    explicit PushMessageHandler() = default;

    PushMessageHandler(const PushMessageHandler&) = delete;
    PushMessageHandler(PushMessageHandler&&) = delete;
    PushMessageHandler& operator=(const PushMessageHandler&) = delete;
    PushMessageHandler& operator=(PushMessageHandler&&) = delete;

    bool CanHandle(MessageType type) const override
    {
        return type == MessageType::Push;
    }

    void Process(Message message) override
    {
        std::cout << "PushMessageHandler  | [" << message.id << "] : " << message.payload << '\n';
        return;
    }
};
