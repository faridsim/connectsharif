#pragma once

#include "../implementations/handlers/EmailMessageHandler.h"
#include "../implementations/handlers/PushMessageHandler.h"
#include "../implementations/handlers/SmsMessageHandler.h"

#include <array>
//??
#include <memory>
//??
#include <stdexcept>

class MessageHandlerFactory
{
public:
    explicit MessageHandlerFactory() = default;

    MessageHandlerFactory(const MessageHandlerFactory&) = delete;
    MessageHandlerFactory(MessageHandlerFactory&&) = delete;
    MessageHandlerFactory& operator=(const MessageHandlerFactory&) = delete;
    MessageHandlerFactory& operator=(MessageHandlerFactory&&) = delete;

    //loops through const array 
    // It returns the raw MessageHandler* stored inside
    //??why bring raw pointer 
    //The caller just borrows a handler pointer to call Process. It must not delete it
    
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
//refrence to const array
//static  function,Allows calling Handlers() without a factory instance; signals it doesn't use object state

    static const std::array<std::unique_ptr<MessageHandler>, 3>& Handlers()
    {
        //static array 
        static const std::array<std::unique_ptr<MessageHandler>, 3> handlers{
            std::make_unique<EmailMessageHandler>(),
            std::make_unique<SmsMessageHandler>(),
            std::make_unique<PushMessageHandler>()
        };

        return handlers;
    }
};







