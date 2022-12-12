//
// Created by Christoph Rohde on 07.12.22.
//

#pragma once

#include <chrono>
#include <thread>
#include <iostream>


template<typename T, typename A, typename I, typename E>


class TimedAction {

private:

    /** Internal state */
    std::string name;
    T value;
    std::function<void(T &value)> action;
    std::chrono::milliseconds interval{};
    std::chrono::milliseconds timeSinceLastAction{};
    bool isRunning{};
    std::thread thread;

    /** Callbacks */
    A actionValue;
    I intervalValue;
    E endValue;

    std::function<void(A &value)> onAction;
    std::function<void(I &value)> onInterval;
    std::function<void(E &value)> onEnd;

public:

    TimedAction(std::string_view const& name,
                std::function<void(T &value)> const& action,
                T &value,
                std::chrono::milliseconds const& interval):

                name(name),
                action(action),
                value(value),
                interval(interval),
                isRunning(false) {


    }

    ~TimedAction() = default;

    auto run() -> void {
        while (isRunning) {
            if (timeSinceLastAction >= interval) {
                timeSinceLastAction = std::chrono::milliseconds(0);

                if (onAction) {
                    onAction(actionValue);
                }

                action(value);
            } else {
                if (onInterval) {
                    onInterval(intervalValue);
                }
                timeSinceLastAction += std::chrono::milliseconds(1);
            }
        }
        if (onEnd) {
            onEnd(endValue);
        }
    }

    auto start() -> void{
        isRunning = true;
        thread = std::thread(&TimedAction::run, this);
    }

    auto stop() -> void {
        isRunning = false;
        thread.join();
    }

    auto restart() -> void {
        stop();
        start();
    }

    auto setAction(std::function<void(T &value)> const& action) -> void {
        this->action = action;
    }

    auto setInterval(std::chrono::milliseconds interval) -> void {
        this->interval = interval;
    }

    auto setOnAction(std::function<void(A &value)> const& onAction, A &actionValue) -> void {
        this->onAction = onAction;
        this->actionValue = actionValue;
    }

    auto setOnInterval(std::function<void(I &value)> const& onInterval, I &intervalValue) -> void {
        this->onInterval = onInterval;
        this->intervalValue = intervalValue;
    }

    auto setOnEnd(std::function<void(E &value)> const& onEnd, E &endValue) -> void {
        this->onEnd = onEnd;
        this->endValue = endValue;
    }

    [[nodiscard]]
    auto getName() const -> std::string_view {
        return name;
    }

    auto getValue() const -> T {
        return value;
    }

    [[nodiscard]] auto getInterval() const -> std::chrono::milliseconds {
        return interval;
    }

    [[nodiscard]] auto getTimeSinceLastAction() const -> std::chrono::milliseconds {
        return timeSinceLastAction;
    }

    [[nodiscard]] auto is_running() const -> bool {
        return isRunning;
    }

};

using SmartTimedAction = TimedAction<std::string, std::string, std::string, std::string>;
