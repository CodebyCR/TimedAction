//
// Created by Christoph Rohde on 18.04.23.
//

#pragma once

#include <iostream>
#include <queue>
#include <functional>

template <typename T>
class AsyncQueue {
protected:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::function<void(T)> on_subscribe;
    std::function<void(T)> on_listen;

    // private because the result of this function is not thread safe
    bool empty() const
    {
        return queue_.empty();
    }

public:
    AsyncQueue() = default;
    AsyncQueue(const AsyncQueue<T> &) = delete;
    AsyncQueue &operator=(const AsyncQueue<T> &) = delete;

    AsyncQueue(AsyncQueue<T> &&other) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        this->queue_ = std::move(other.queue_);
    }

    virtual ~AsyncQueue() = default;

    auto size() const -> unsigned long
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    auto pop() -> std::optional<T>
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return {};
        }
        T tmp = queue_.front();
        queue_.pop();

        if (on_listen)
        {
            on_listen(tmp);
        }

        return tmp;
    }

    auto push(const T &item) -> void
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);

        if (on_subscribe)
        {
            on_subscribe(item);
        }
    }

    auto clear() -> void
    {
        while (this->size() > 0)
        {
            this->pop();
        }
    }

    auto listen(std::function<void(T)> on_listen) -> void
    {
        this->on_listen = on_listen;
    }

    auto subscribe(std::function<void(T)> on_subscribe) -> void
    {
        this->on_subscribe = on_subscribe;
    }
};
