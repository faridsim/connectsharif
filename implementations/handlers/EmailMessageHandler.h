#pragma once

#include "../../abstractions/MessageHandler.h"

#include <iostream>

class EmailMessageHandler final : public MessageHandler
{
public:
    bool CanHandle(MessageType type) const override
    {
        return type == MessageType::Email;
    }

    void Process(Message message) override
    {
        std::cout << "EmailMessageHandler  | [" << message.id << "] : " << message.payload << '\n';
        return;
    }
};
