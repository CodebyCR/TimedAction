//
// Created by Christoph Rohde on 07.12.22.
//

#pragma once

#include <chrono>
#include <thread>
#include <iostream>
#include <variant>
#include "I_TimedAction.hpp"
#include "../Notification/JobLog.hpp"


template<typename T, typename A, typename I, typename E>


class TimedAction : public I_TimedAction {

private:

    /** Internal state */
    T value;
    std::function<void(T &value)> action;
    std::chrono::milliseconds timeSinceLastAction{};
    bool isRunning;
    std::thread thread;

    /** Execution times */
    std::chrono::milliseconds interval;

    [[Deprecated ("use Callback<A, I, E> instead <A, I, E>") ]]
    /** Callbacks */
    A actionValue;
    I intervalValue;
    E endValue;

    [[Deprecated ("use Callback<A, I, E> instead of <A, I, E>") ]]
    std::function<void(A &value)> onAction;
    std::function<void(I &value)> onInterval;
    std::function<void(E &value)> onEnd;

public:

    TimedAction(std::string_view const& name,
                std::function<void(T &value)> const& action,
                T &value,
                std::chrono::milliseconds interval):

                action(action),
                value(value),
                interval(interval),
                isRunning(false) {

        this->name = name;
    }

    TimedAction(std::string_view const& name,
                std::function<void(T &value)> const& action,
                T &value,
                std::vector<std::tm>& execution_times):

            action(action),
            value(value),
            isRunning(false) {

        this->name = name;
        this->execution_times(execution_times);
    }

    TimedAction(std::string_view const& name,
                std::function<void(T &value)> const& action,
                T &value,
                Cron& corn ):

            action(action),
            value(value),
            isRunning(false) {
        this->name = name;
        this->execution_times = corn.get_execution_times();
    }


    ~TimedAction() override = default;



    auto run() -> void {

        if (interval != std::chrono::milliseconds{}) {
            run_as_interval(interval);
        }

        if (!execution_times.empty()) {
            run_as_times(execution_times);
        }

    }

    auto run_as_times(const std::vector<std::tm> &vector) {

        for(auto time : vector) {
            auto now = std::chrono::system_clock::now();
            auto time_point = std::chrono::system_clock::from_time_t(std::mktime( &time));

            while (isRunning) {
                if (now == time_point) {
                    timeSinceLastAction = std::chrono::milliseconds(0);

                    if (onAction) {
                        onAction(actionValue);
                    }

//                    action(value);
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
    }

    auto run_as_interval(const std::chrono::milliseconds &interval) {
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

    auto start()  -> void override {
        isRunning = true;
        thread = std::thread(&TimedAction::run, this);
        thread.detach(); // ? TODO: test this
    }

    auto stop() -> void override{
        isRunning = false;
        thread.join();
    }

    auto restart() -> void override {
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
    auto getName() const -> std::string_view override {
        return name;
    }

    auto getValue() const -> T {
        return value;
    }

//    [[nodiscard]] auto getInterval() const -> std::chrono::milliseconds {
//        return interval;
//    }

    [[nodiscard]]
    auto getTimeSinceLastAction() const -> std::chrono::milliseconds {
        return timeSinceLastAction;
    }

    [[nodiscard]]
    auto is_running() const -> bool override {
        return isRunning;
    }

    [[nodiscard]]
    auto get_execution_times() const -> std::vector<std::tm> override {
        return execution_times;
    }

    auto finished() -> std::future<Notification>  {
        auto timeStamp = std::chrono::system_clock::now();


        return std::async(std::launch::async, [this, &timeStamp] {
            auto jobLog = JobLog(this->name, timeStamp);
            jobLog.INFO("Test -> INFO");
            jobLog.WARNING("Test -> WARNING");
            jobLog.FAILURE("Test -> FAILURE");
            jobLog.SUCCESS("Test -> SUCCESS");

            return jobLog;
        });
    }

};

using SmartTimedAction = TimedAction<std::string, std::string, std::string, std::string>;
