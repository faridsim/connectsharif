#pragma once

#include "MessageHandlerFactory.h"
#include "../abstractions/MessageObserver.h"
#include "../models/Message.h"

#include <list>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

class MessageProcessor
{
    
public:

    explicit MessageProcessor() = default;

    MessageProcessor(const MessageProcessor&) = delete;
    MessageProcessor(MessageProcessor&&) = delete;
    MessageProcessor& operator=(const MessageProcessor&) = delete;
    MessageProcessor& operator=(MessageProcessor&&) = delete;


private:
    std::mutex mutex;
    std::list<std::pair<MessageType, MessageObserver*>> observers;
    static inline std::once_flag factoryOnce;
    static inline std::unique_ptr<MessageHandlerFactory> factory;


    void Subscribe(MessageType type, MessageObserver& observer)
    {
        std::lock_guard<std::mutex> lock(mutex);
        observers.emplace_back(type, &observer);
    }



    void UnSubscribe(MessageType type, MessageObserver& observer)
    {
        std::lock_guard<std::mutex> lock(mutex);
        observers.remove(std::pair<MessageType, MessageObserver*>(type, &observer));
    }




    void Process(const Message& message)
    {
        std::call_once(factoryOnce, []
        {
            factory = std::make_unique<MessageHandlerFactory>();
        });

        factory->ResolveHandler(message)->Process(message);

        for (MessageObserver* observer : Interested(message.type))
            observer->Update(message);
    }


    std::vector<MessageObserver*> Interested(MessageType type)
    {
        std::vector<MessageObserver*> interested;
        std::lock_guard<std::mutex> lock(mutex);
        for (const std::pair<MessageType, MessageObserver*>& entry : observers)
        {
            if (entry.first == type)
                interested.push_back(entry.second);
        }
        return interested;
    }





};
