//
// Created by Christoph Rohde on 18.04.23.
//

#pragma once

#include <iostream>
#include <deque>
#include <functional>

/////////////////////////////////////////////////
/// * AsyncQueue * | Author: Christoph Rohde ///
///////////////////////////////////////////////

template <typename T>
class AsyncQueue {

protected:
    std::deque<T> queue_;
    mutable std::mutex mutex_;


    // private because the result of this function is not thread safe
    bool empty() const
    {
        return queue_.empty();
    }

public:
    std::function<void(T)> on_subscribe;
    std::function<void(T)> on_listen;

    AsyncQueue() = default;
    AsyncQueue(const AsyncQueue<T> &) = delete;
    AsyncQueue &operator=(const AsyncQueue<T> &) = delete;

    AsyncQueue(AsyncQueue<T> &&other) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        this->queue_ = std::move(other.queue_);
    }

    virtual ~AsyncQueue() = default;

    friend auto operator<=>(const AsyncQueue<T> &lhs, const AsyncQueue<T> &rhs) = default;

    /// iterator
    auto begin() const -> decltype(queue_.begin())
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.begin();
    }

    auto end() const -> decltype(queue_.end())
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.end();
    }

    auto size() const -> unsigned long
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }


    auto push(const T &item) -> void
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(item);

        if (on_subscribe)
        {
            on_subscribe(item);
        }
    }

    auto pop() -> std::optional<T>
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return {};
        }
        T tmp = queue_.front();
        queue_.pop_front();

        if (on_listen)
        {
            on_listen(tmp);
        }

        return tmp;
    }

    /// ? use pop without listening instead ?
    auto clear() -> void
    {
        while (this->size() > 0)
        {
            this->pop();
        }
    }

    /// ! not thread safe
    /// -> result is not significant
    auto contains(const T &item) const -> bool
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return std::find(queue_.begin(), queue_.end(), item) != queue_.end();
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
