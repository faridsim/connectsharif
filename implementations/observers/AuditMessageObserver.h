#pragma once

#include "../../abstractions/MessageObserver.h"

#include <iostream>

class AuditMessageObserver final : public MessageObserver
{
public:
    void Update(const Message& message) override
    {
        // prints blue
        std::cout << "\033[34mAudit  | [" << message.id << "] : " << message.payload << "\033[0m\n";
    }
};
