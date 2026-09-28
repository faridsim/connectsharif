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

    void Process(Message) override
    {
        // No member state, so no lock. Concurrent cout lines may interleave; that is not a data race.
        std::cout << "Sending push...\n";
        return;
    }
};
