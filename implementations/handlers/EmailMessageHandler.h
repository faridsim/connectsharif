#pragma once

#include "../../abstractions/MessageHandler.h"

#include <iostream>

class EmailMessageHandler final : public MessageHandler
{
public:
    explicit EmailMessageHandler() = default;

    EmailMessageHandler(const EmailMessageHandler&) = delete;
    EmailMessageHandler(EmailMessageHandler&&) = delete;
    EmailMessageHandler& operator=(const EmailMessageHandler&) = delete;
    EmailMessageHandler& operator=(EmailMessageHandler&&) = delete;
    //true if type is email
    //not mandatory,but gives compile time error instead of creating new fucntion,when signatrues dont match (fogot const)
    bool CanHandle(MessageType type) const override
    {
        return type == MessageType::Email;
    }


    void Process(Message) override
    {
        // No member state, so no lock. Concurrent cout lines may interleave; that is not a data race.
        std::cout << "Sending email...\n";
        return;
    }
};



