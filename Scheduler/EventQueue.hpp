//
// Created by Christoph Rohde on 18.04.23.
//

#pragma once

#include "../Container/AsyncQueue.hpp"
#include "../TimedAction_Types/I_TimedAction.hpp"
#include <iostream>

class EventQueue : public AsyncQueue<I_TimedAction*> {
private:
    std::vector<std::tm> executionTimes;

public:
    EventQueue() = default;
    EventQueue(const EventQueue&) = default;
    EventQueue& operator=(const EventQueue&) = default;

    EventQueue(EventQueue&& other) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        this->queue_ = std::move(other.queue_);
    }

    ~EventQueue() override = default;

    auto get_execution_times() const -> std::vector<std::tm> {
        return executionTimes;
    }
};

/// Example implementation of the EventQueue
// auto eventQueue = EventQueue();
// eventQueue.on_subscribe = [](I_TimedAction action) {
//     std::cout << "EventQueue: subscribed to " << action.getName() << std::endl;
// };

// eventQueue.on_listen = [](I_TimedAction action) {
//     std::cout << "EventQueue: listened to " << action.getName() << std::endl;
// };

// eventQueue.push(action);
// auto action = eventQueue.pop();
// if (action.has_value()) {
//     std::cout << "EventQueue: popped " << action.value().getName() << std::endl;
// } else {
//     std::cout << "EventQueue: nothing to pop" << std::endl;
// }
