#pragma once

#include "../../abstractions/MessageObserver.h"

#include <iostream>

class LengthMessageObserver final : public MessageObserver
{
public:
    void Update(const Message& message) override
    {
        // prints green
        std::cout << "\033[32mLength | [" << message.id << "] : " << message.payload.size() << " chars\033[0m\n";
    }
};
