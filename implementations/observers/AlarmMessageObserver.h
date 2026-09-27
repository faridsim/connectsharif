#pragma once

#include "../../abstractions/MessageObserver.h"

#include <iostream>

class AlarmMessageObserver final : public MessageObserver
{
public:
    void Update(const Message& message) override
    {
        if (message.payload != "alert")
            return;

        // prints red
        std::cout << "\033[31mAlarm  | [" << message.id << "] : ALERT\033[0m\n";
    }
};
