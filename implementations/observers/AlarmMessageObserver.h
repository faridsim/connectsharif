#pragma once

#include "../../abstractions/MessageObserver.h"

#include <iostream>

class AlarmMessageObserver final : public MessageObserver
{
public:
    explicit AlarmMessageObserver() = default;

    AlarmMessageObserver(const AlarmMessageObserver&) = delete;
    AlarmMessageObserver(AlarmMessageObserver&&) = delete;
    AlarmMessageObserver& operator=(const AlarmMessageObserver&) = delete;
    AlarmMessageObserver& operator=(AlarmMessageObserver&&) = delete;

    void Update(const Message& message) override
    {
        // No member state, so no lock. Concurrent cout lines may interleave; that is not a data race.
        if (message.payload != "alert")
            return;

        // prints red
        std::cout << "\033[31mAlarm  | [" << message.id << "] : ALERT\033[0m\n";
    }
};

