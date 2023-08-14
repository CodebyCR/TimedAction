//
// Created by Christoph Rohde on 13.12.22.
//

#pragma once

#include "../Notification/JobLog.hpp"
#include "../Interfaces/I_TimedAction.hpp"
#include <chrono>
#include <iostream>
#include <thread>


template<typename T>
class Light_TimedAction : public I_TimedAction {
private:
    /** Internal state */
    bool value;
    std::function<void(bool& value)> action;
    std::chrono::milliseconds interval {};
    std::chrono::milliseconds timeSinceLastAction {};
    bool isRunning {};
    std::thread thread;


public:
    Light_TimedAction(std::string_view const& name,
                      std::function<void(T& value)> const& action,
                      T& value,
                      std::chrono::milliseconds const& interval)
        : action(static_cast<const std::function<void(bool&)>>(action)),
                                                                   value(value),
                                                                   interval(interval),
                                                                   isRunning(false) {
        this->name = name;
    }

    ~Light_TimedAction() override = default;

    auto run() -> void {
        while(isRunning) {
            if(timeSinceLastAction >= interval) {
                timeSinceLastAction = std::chrono::milliseconds(0);

                action(value);
            }
            else {
                timeSinceLastAction += std::chrono::milliseconds(1);
            }
        }
    }

    auto finished() -> std::future<Message> {    // override {
        return std::async(std::launch::async, []() -> Message {
            auto jobLog = JobLog("TimedActionLib", "DATE");
            jobLog.SUCCESS("TimedActionLib finished");

            return jobLog;
        });
    }

    auto start() -> void override {
        isRunning = true;
        thread = std::thread(&Light_TimedAction::run, this);
    }

    auto stop() -> void override {
        isRunning = false;
        thread.join();
    }

//    auto restart() -> void override {
//        stop();
//        start();
//    }

    auto setAction(std::function<void(T& value)> const& action) -> void {
        this->action = action;
    }

    auto setInterval(std::chrono::milliseconds interval) -> void {
        this->interval = interval;
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

//    [[nodiscard]] auto is_running() const -> bool override {
//        return isRunning;
//    }

//    [[nodiscard]] auto get_execution_times() const -> std::vector<std::tm> override {
//        return this->cron.get_execution_times();
//    }
};
