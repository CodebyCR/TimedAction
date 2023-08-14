//
// Created by Christoph Rohde on 13.12.22.
//

#pragma once

#include "../Cron/Cron.hpp"
#include "../Notification/JobLog.hpp"
#include "Message.hpp"
#include <future>
#include <iostream>

class I_TimedAction {
protected:
    std::string name;
    std::vector<std::tm> execution_times = {};
    std::function<void(void)> action;
    I_ExecutionTimer* execution_timer;


public:

    I_TimedAction(std::string_view name,
                  std::vector<std::tm>const& execution_times,
                  std::function<void(void)>const& action)
        : name(name),
          execution_times(execution_times),
          action(action) {}

    I_TimedAction(std::string_view name,
                  I_ExecutionTimer* execution_timer,
                  std::function<void(void)> const& action)
        : name(name),
          action(action) {
        this->execution_timer = execution_timer;
    }


    virtual ~I_TimedAction() = default;

    //virtual auto finished() -> std::future<Message> = 0;

    virtual auto start() -> void = 0;

    virtual auto stop() -> void = 0;
//
//    virtual auto restart() -> void = 0;
//
//    [[nodiscard]]
//    virtual auto is_running() const -> bool = 0;


    /// << ABSTRACT >>

    [[nodiscard]]
    auto getName() -> std::string_view {
        return this->name;
    }

    /// This methode returns a future which contains the JobLog.
    [[nodiscard]]
    auto finished() -> std::future<Message> {    // override {
        return std::async(std::launch::async, [this]() -> Message {
            auto jobLog = JobLog(this->name, "DATE");
            jobLog.SUCCESS(this->name + " finished.");

            return jobLog;
        });
    }

    [[nodiscard]]
    auto get_execution_times()  -> std::vector<std::tm> {
        if(execution_timer) {
            return execution_timer->get_execution_times();
        }

        return this->execution_times;
    }

};
