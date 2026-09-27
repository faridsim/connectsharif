#pragma once

#include "../models/Message.h"

class MessageObserver
{
public:
    MessageObserver() = default;
    MessageObserver(const MessageObserver&) = delete;
    MessageObserver(MessageObserver&&) = delete;
    MessageObserver& operator=(const MessageObserver&) = delete;
    MessageObserver& operator=(MessageObserver&&) = delete;

    virtual ~MessageObserver() = default;

    virtual void Update(const Message& message) = 0;
};
