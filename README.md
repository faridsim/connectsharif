# Message Processor

A C++17 message routing system with handlers, observers, and thread-safe processing.

## Architecture
- **Message** — holds `id`, `type` (`Email`/`Sms`/`Push`), and `payload`.
- **MessageHandler** — abstract base with `CanHandle(type)` and `Process(message)`.
- **MessageHandlerFactory** — owns one handler per type, resolves by `CanHandle`.
- **MessageObserver** — abstract base with `Update(message)`; receives notifications.
- **MessageProcessor** — subscribes observers, deduplicates by id, dispatches to handlers, notifies interested observers.
- **Concurrency** — `mutex` guards subscriptions and id set; handlers and observers run unlocked.