#include "core/MessageProcessor.h"
#include "implementations/observers/AlarmMessageObserver.h"
#include "implementations/observers/AuditMessageObserver.h"
#include "implementations/observers/LatestMessageObserver.h"
#include "implementations/observers/LengthMessageObserver.h"
#include "models/Message.h"

int main()
{
    // instantiate observers
    AuditMessageObserver audit;
    AlarmMessageObserver alarm;
    LengthMessageObserver length;
    LatestMessageObserver latest;

    // create a list of observers
    MessageObserver* observers[] = {&audit, &alarm, &length, &latest};

    // a list of all message types
    MessageType types[] = {MessageType::Email, MessageType::Sms, MessageType::Push};

    
    // instantiate the message processor
    MessageProcessor processor;

    // every observer subscribes to all message types
    for (MessageType type : types)
    {
        for (MessageObserver* observer : observers)
            processor.Subscribe(type, *observer);
    }

    processor.Process(Message(1, MessageType::Email, "hello world"));
    processor.Process(Message(2, MessageType::Sms, "ping"));
    processor.Process(Message(3, MessageType::Push, "alert"));

    processor.UnSubscribe(MessageType::Email, alarm);
    processor.Process(Message(4, MessageType::Email, "alert"));
    //no alarm message should be printed

    return 0;
}
