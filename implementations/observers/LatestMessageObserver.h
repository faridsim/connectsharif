#pragma once

#include "../../abstractions/MessageObserver.h"

#include <iostream>
#include <string>

class LatestMessageObserver final : public MessageObserver
{
public:
    void Update(const Message& message) override
    {
        // prints yellow
        std::cout << "\033[33mLatest | [" << message.id << "] : ";
        if (!seen)
            std::cout << "now \"" << message.payload << "\"";
        else
            std::cout << "was \"" << previous << "\", now \"" << message.payload << "\"";
        std::cout << "\033[0m\n";

        previous = message.payload;
        seen = true;
    }

private:
    bool seen = false;
    std::string previous;
};
