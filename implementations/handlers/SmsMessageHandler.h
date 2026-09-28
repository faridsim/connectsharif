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

    void Process(Message) override
    {
        // No member state, so no lock. Concurrent cout lines may interleave; that is not a data race.
        std::cout << "Sending SMS...\n";
        return;
    }
};
