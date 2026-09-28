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
        //One thread calls Subscribe while another calls Process
        //threads call Subscribe together, for the same observer or for different ones
        
        std::lock_guard<std::mutex> lock(mutex);
        //list-pair 
        observers.emplace_back(type, &observer);
    }


    void UnSubscribe(MessageType type, MessageObserver& observer)
    {
        //???Process that already copied this pointer can still call Update.
        // Removal only hides the observer from a Process that locks after this returns.
        std::lock_guard<std::mutex> lock(mutex);
        observers.remove(std::pair<MessageType, MessageObserver*>(type, &observer));
    }

    bool Process(const Message& message)
    {
        //Later calls: flag is already set → lambda is skipped entirely.
        //set thge factory
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

        //interested is a local vector (created fresh on every Process call) 
        //lg the subset of subscribed observers whose type matches the incoming message's type

        std::vector<MessageObserver*> interested;
        bool duplicate = false;
        {
            
            // update profile shows them blocking each other.
            // Check and insert stay in this block. A gap between them lets two
            // threads both accept the same id.
            std::lock_guard<std::mutex> lock(mutex);
            //processedIds.insert(5).second  // true  → 5 wasn't there, now it is
            //processedIds.insert(5).second  // false → 5 was already there, nothing changed



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


   //guard observers and process ids
    std::mutex mutex;
   //Process scans it to find observers to notify
   //std::vector is faster choice for this 
    std::list<std::pair<MessageType, MessageObserver*>> observers;

    std::unordered_set<int> processedIds;
    //?flag to be raisen once
    static inline std::once_flag factoryOnce;


    static inline std::unique_ptr<MessageHandlerFactory> factory;
};
