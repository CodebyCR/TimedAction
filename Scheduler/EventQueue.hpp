//
// Created by Christoph Rohde on 18.04.23.
//

#pragma once

#include <iostream>
#include "../Container/AsyncQueue.hpp"
#include "../TimedAction_Types/I_TimedAction.hpp"

class EventQueue : public AsyncQueue<I_TimedAction> {
    private:
        std::vector<std::tm> executionTimes;

    public:
        EventQueue() = default;
        EventQueue(const EventQueue &) = delete;
        EventQueue &operator=(const EventQueue &) = delete;

        EventQueue(EventQueue &&other) noexcept {
            std::lock_guard<std::mutex> lock(mutex_);
            this->queue_ = std::move(other.queue_);
        }

        ~EventQueue() override = default;

        auto get_execution_times() const -> std::vector<std::tm> {
            return executionTimes;
        }

};
