#include "core/MessageProcessor.h"
#include "implementations/observers/AlarmMessageObserver.h"
#include "implementations/observers/AuditMessageObserver.h"
#include "implementations/observers/LatestMessageObserver.h"
#include "implementations/observers/LengthMessageObserver.h"
#include "models/Message.h"

#include <array>

int main()
{
    // instantiate observers
    AuditMessageObserver audit;
    AlarmMessageObserver alarm;
    LengthMessageObserver length;
    LatestMessageObserver latest;

    // create a list of observers (base-class pointers)
    const std::array<MessageObserver*, 4> observers{
        &audit, &alarm, &length, &latest
    };


    // a list of all message types
    const std::array<MessageType, 3> types{
        MessageType::Email,
        MessageType::Sms,
        MessageType::Push
    };


    // instantiate the message processor
    MessageProcessor processor;
    
    //susbcribing 12 items
    // every observer subscribes to all message types
    //enum values are cheap to copy ,so we dont use pointers
    for (MessageType type : types)
        for (MessageObserver* observer : observers)
            processor.Subscribe(type, *observer);


    
    processor.Process(Message(1, MessageType::Email, "hello world"));
    processor.Process(Message(2, MessageType::Sms, "ping"));
    processor.Process(Message(3, MessageType::Push, "alert"));

    processor.UnSubscribe(MessageType::Email, alarm);
    processor.Process(Message(4, MessageType::Email, "alert"));
    // no alarm message should be printed

    processor.Process(Message(1, MessageType::Email, "again"));
    processor.Process(Message(5, MessageType::Email, ""));
    processor.Process(Message(6, static_cast<MessageType>(99), "nope"));

    return 0;
}


