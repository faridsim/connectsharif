#pragma once

#include "MessageHandlerFactory.h"
#include "../abstractions/MessageObserver.h"
#include "../models/Message.h"

#include <exception>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_set>
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

    void Subscribe(MessageType type, MessageObserver& observer)
    {
        // Process may be copying this list on another thread. The lock publishes
        // the new pair whole, so that copy never sees a half-written entry.
        std::lock_guard<std::mutex> lock(mutex);
        observers.emplace_back(type, &observer);
    }
    
    void UnSubscribe(MessageType type, MessageObserver& observer)
    {
        // A Process that already copied this pointer can still call Update.
        // Removal only hides the observer from a Process that locks after this returns.
        std::lock_guard<std::mutex> lock(mutex);
        observers.remove(std::pair<MessageType, MessageObserver*>(type, &observer));
    }

    bool Process(const Message& message)
    {
        // The caller must not change message while this runs. Several threads may
        // each pass their own Message; this function does not share that object.
        // call_once publishes factory to every thread. Handlers keep no mutable
        // members, so sharing them needs no lock of their own.
        std::call_once(factoryOnce, []
        {
            factory = std::make_unique<MessageHandlerFactory>();
        });

        // Printing is the failure report. The bool is there for the tests.
        if (message.payload.empty())
        {
            std::cout << "failed: empty payload\n";
            return false;
        }

        std::vector<MessageObserver*> interested;
        bool duplicate = false;
        {
            // ponytail: one lock for ids and the observer list. Split them if an
            // update profile shows them blocking each other.
            // Check and insert stay in this block. A gap between them lets two
            // threads both accept the same id.
            std::lock_guard<std::mutex> lock(mutex);
            if (!processedIds.insert(message.id).second)
                duplicate = true;
            else
            {
                for (const std::pair<MessageType, MessageObserver*>& entry : observers)
                {
                    if (entry.first == message.type)
                        interested.push_back(entry.second);
                }
            }
        }

        if (duplicate)
        {
            std::cout << "failed: duplicate id\n";
            return false;
        }

        // Handler and Update run unlocked. Either may call Subscribe, and this
        // mutex is not recursive, so holding it across the callback deadlocks.
        try
        {
            factory->ResolveHandler(message)->Process(message);
        }
        catch (const std::exception&)
        {
            {
                std::lock_guard<std::mutex> lock(mutex);
                processedIds.erase(message.id);
            }
            std::cout << "failed: unknown type\n";
            return false;
        }

        for (MessageObserver* observer : interested)
            observer->Update(message);

        return true;
    }

private:
    std::mutex mutex;
    std::list<std::pair<MessageType, MessageObserver*>> observers;
    std::unordered_set<int> processedIds;
    static inline std::once_flag factoryOnce;
    static inline std::unique_ptr<MessageHandlerFactory> factory;
};
