#pragma once

#include "../../abstractions/MessageObserver.h"

#include <iostream>
#include <mutex>
#include <string>

class LatestMessageObserver final : public MessageObserver
{
public:
    explicit LatestMessageObserver() = default;

    LatestMessageObserver(const LatestMessageObserver&) = delete;
    LatestMessageObserver(LatestMessageObserver&&) = delete;
    LatestMessageObserver& operator=(const LatestMessageObserver&) = delete;
    LatestMessageObserver& operator=(LatestMessageObserver&&) = delete;

    void Update(const Message& message) override
    {
        // Process drops its lock before calling here, so two Updates can overlap.
        // seen and previous are a std::string write; without this lock that is a data race.
        std::lock_guard<std::mutex> lock(mutex);

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
    std::mutex mutex;
    bool seen = false;
    std::string previous;
};
