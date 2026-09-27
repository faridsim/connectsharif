#pragma once

#include "../../abstractions/MessageHandler.h"

#include <iostream>

class SmsMessageHandler final : public MessageHandler
{
public:
    bool CanHandle(MessageType type) const override
    {
        return type == MessageType::Sms;
    }

    void Process(Message message) override
    {
        std::cout << "SmsMessageHandler  | [" << message.id << "] : " << message.payload << '\n';
        return;
    }
};
