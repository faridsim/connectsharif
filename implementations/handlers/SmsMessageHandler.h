#pragma once

#include "../../abstractions/MessageHandler.h"

#include <iostream>

class SmsMessageHandler final : public MessageHandler
{
public:
    explicit SmsMessageHandler() = default;

    SmsMessageHandler(const SmsMessageHandler&) = delete;
    SmsMessageHandler(SmsMessageHandler&&) = delete;
    SmsMessageHandler& operator=(const SmsMessageHandler&) = delete;
    SmsMessageHandler& operator=(SmsMessageHandler&&) = delete;

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
