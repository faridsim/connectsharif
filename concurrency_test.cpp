#include "core/MessageProcessor.h"
#include "models/Message.h"

#include <atomic>
#include <cassert>
#include <mutex>
#include <thread>
#include <vector>

class CountingObserver final : public MessageObserver
{
public:
    void Update(const Message&) override
    {
        // Process calls this from several threads after releasing its lock.
        std::lock_guard<std::mutex> lock(mutex);
        ++count;
    }

    int Count()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return count;
    }

private:
    std::mutex mutex;
    int count = 0;
};

static void SameIdOneSuccess()
{
    MessageProcessor processor;
    constexpr int threadCount = 8;
    std::atomic<int> successes{0};
    std::vector<std::thread> workers;
    for (int index = 0; index < threadCount; ++index)
    {
        workers.emplace_back([&processor, &successes]
        {
            if (processor.Process(Message(1, MessageType::Email, "same")))
                successes.fetch_add(1);
        });
    }
    for (std::thread& worker : workers)
        worker.join();
    assert(successes.load() == 1);
}

static void DistinctIdsAllDelivered()
{
    MessageProcessor processor;
    CountingObserver observer;
    processor.Subscribe(MessageType::Email, observer);

    constexpr int messageCount = 8;
    std::atomic<int> successes{0};
    std::vector<std::thread> workers;
    for (int id = 1; id <= messageCount; ++id)
    {
        workers.emplace_back([&processor, &successes, id]
        {
            if (processor.Process(Message(id, MessageType::Email, "n")))
                successes.fetch_add(1);
        });
    }
    for (std::thread& worker : workers)
        worker.join();
    assert(successes.load() == messageCount);
    assert(observer.Count() == messageCount);
}

static void UnSubscribeHidesLaterProcess()
{
    MessageProcessor processor;
    CountingObserver emailObserver;
    CountingObserver smsObserver;
    processor.Subscribe(MessageType::Email, emailObserver);
    processor.Subscribe(MessageType::Sms, smsObserver);

    std::atomic<bool> started{false};
    std::thread smsWorker([&processor, &started]
    {
        started.store(true);
        for (int id = 100; id < 140; ++id)
            processor.Process(Message(id, MessageType::Sms, "busy"));
    });
    while (!started.load())
        std::this_thread::yield();

    processor.UnSubscribe(MessageType::Email, emailObserver);
    bool accepted = processor.Process(Message(1, MessageType::Email, "after"));
    smsWorker.join();

    assert(accepted);
    assert(emailObserver.Count() == 0);
}

static void RejectsInvalidDuplicateAndUnknown()
{
    MessageProcessor processor;
    CountingObserver observer;
    processor.Subscribe(MessageType::Email, observer);

    assert(processor.Process(Message(1, MessageType::Email, "once")));
    assert(!processor.Process(Message(1, MessageType::Email, "twice")));
    assert(!processor.Process(Message(2, MessageType::Email, "")));
    assert(!processor.Process(Message(3, static_cast<MessageType>(99), "nope")));
    assert(observer.Count() == 1);
}

int main()
{
    SameIdOneSuccess();
    DistinctIdsAllDelivered();
    UnSubscribeHidesLaterProcess();
    RejectsInvalidDuplicateAndUnknown();
    return 0;
}
