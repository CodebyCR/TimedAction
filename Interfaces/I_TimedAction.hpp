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
    std::vector<std::tm> execution_times;

public:
    virtual ~I_TimedAction() = default;

    //virtual auto finished() -> std::future<Message> = 0;

    virtual auto start() -> void = 0;

    virtual auto stop() -> void = 0;

    virtual auto restart() -> void = 0;

    [[nodiscard]]
    virtual auto is_running() const -> bool = 0;

    [[nodiscard]]
    virtual auto get_execution_times() const -> std::vector<std::tm> = 0;


    /// << ABSTRACT >>

    [[nodiscard]]
    auto getName() -> std::string_view {
        return this->name;
    }

    /// This methode returns a future which contains the JobLog.
    [[nodiscard]]
    virtual auto finished() -> std::future<Message> {    // override {
        return std::async(std::launch::async, [this]() -> Message {
            auto jobLog = JobLog(this->name, "DATE");
            jobLog.SUCCESS(this->name + " finished.");

            return jobLog;
        });
    }
};
