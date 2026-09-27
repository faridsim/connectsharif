#pragma once

#include "../../abstractions/MessageObserver.h"

#include <iostream>

class LengthMessageObserver final : public MessageObserver
{
public:
    explicit LengthMessageObserver() = default;

    LengthMessageObserver(const LengthMessageObserver&) = delete;
    LengthMessageObserver(LengthMessageObserver&&) = delete;
    LengthMessageObserver& operator=(const LengthMessageObserver&) = delete;
    LengthMessageObserver& operator=(LengthMessageObserver&&) = delete;

    void Update(const Message& message) override
    {
        // prints green
        std::cout << "\033[32mLength | [" << message.id << "] : " << message.payload.size() << " chars\033[0m\n";
    }
};
