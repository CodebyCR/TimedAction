//
// Created by Christoph Rohde on 13.12.22.
//

#pragma once

#include <iostream>
#include <future>
#include "../Cron/Cron.hpp"
#include "../Notification/Notification.hpp"
#include "../Notification/JobLog.hpp"

class I_TimedAction {
protected:
    std::string name;

    std::vector<std::tm> execution_times;

public:

    virtual ~ I_TimedAction() = default;

    //virtual auto finished() -> std::future<Notification> = 0;

    virtual auto start() -> void = 0;

    virtual auto stop() -> void = 0;

    virtual auto restart() -> void = 0;

    [[nodiscard]]
    virtual auto is_running() const -> bool = 0;

    [[nodiscard]]
    virtual auto getName() const -> std::string_view = 0;

    [[nodiscard]]
    virtual auto get_execution_times() const -> std::vector<std::tm> = 0;


    // << Abstract >>

    auto execution_time_count_message() -> std::string
    {
        if(this->get_execution_times().empty()) {
            return std::string("TimeTable -> WARNING: no execution times for '")
                    .append(this->getName())
                    .append(std::string("'."));
        }

        const auto execution_count = this->get_execution_times().size();
        const bool is_single_execution = execution_count == 1;

        auto ss = std::stringstream();
        ss << "TimeTable: put '" << name << "' for execution at " << std::asctime(&this->execution_times[0]);
        is_single_execution
        ? ss << "." << std::flush
        : ss << " and " << execution_count -1 << " more times." << std::flush;
        return ss.str();
    }

    /// This methode returns a future which contains the JobLog.
    auto finished() -> std::future<Notification> { // override {

        return std::async(std::launch::async, [this]() -> Notification {
            auto jobLog = JobLog(this->name, "DATE");
            jobLog.SUCCESS(this->name + " finished.");

            return jobLog;
        });
    }


};
