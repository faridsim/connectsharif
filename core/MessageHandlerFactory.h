#pragma once

#include "../implementations/handlers/EmailMessageHandler.h"
#include "../implementations/handlers/PushMessageHandler.h"
#include "../implementations/handlers/SmsMessageHandler.h"

#include <array>
#include <memory>
#include <stdexcept>

class MessageHandlerFactory
{
public:
    explicit MessageHandlerFactory() = default;

    MessageHandlerFactory(const MessageHandlerFactory&) = delete;
    MessageHandlerFactory(MessageHandlerFactory&&) = delete;
    MessageHandlerFactory& operator=(const MessageHandlerFactory&) = delete;
    MessageHandlerFactory& operator=(MessageHandlerFactory&&) = delete;

    MessageHandler* ResolveHandler(const Message& message)
    {
        for (auto& handler : Handlers())
        {
            if (handler->CanHandle(message.type))
                return handler.get();
        }

        throw std::runtime_error("no handler for message");
    }

private:
    static const std::array<std::unique_ptr<MessageHandler>, 3>& Handlers()
    {
        static const std::array<std::unique_ptr<MessageHandler>, 3> handlers{
            std::make_unique<EmailMessageHandler>(),
            std::make_unique<SmsMessageHandler>(),
            std::make_unique<PushMessageHandler>()
        };

        return handlers;
    }
};
