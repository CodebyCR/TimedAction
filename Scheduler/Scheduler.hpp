//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include "../Cron/CronInterpreter.hpp"
#include "../Interfaces/I_TimedAction.hpp"
#include "../TimedAction_Types/ScheduledAction.hpp"
#include "../Utilities/Dispacher.hpp"
#include "EventQueue.hpp"
#include "JobManager.hpp"
#include "TimeTable.hpp"
#include "WatchableList.hpp"
#include "Watcher.hpp"
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>


class Scheduler {
public:
    virtual ~Scheduler() = default;

    // Singleton
    static auto get_instance() -> std::shared_ptr<Scheduler> {
        static Scheduler instance;
        return std::make_shared<Scheduler>(instance);
    }

    Scheduler(Scheduler const&) = default;
    void operator=(Scheduler const&) = delete;

    auto add(std::shared_ptr<I_TimedAction> const& action) -> void {
        timeTable_ptr->add(action);
    }

    auto add(ActionCapsule const& action) -> void {
        timeTable_ptr->add(std::make_shared<ScheduledAction>(action));
    }

    auto add(std::shared_ptr<Watchable> const& watchable) -> void {
        if(!watchable) {
            std::cout << "[ WatchableList | ERROR ] -> uninitialised Watchable." << std::endl;
            return;
        }

        this->watchables.emplace_back(watchable);
    }

    /// Starts a new independent thread that watches the watchables.
    auto start() -> void {
        watcher.isRunning = true;
        auto watcher_thread = watcher.open_watch_thread(watchables);
        watcher_thread.detach();
        std::cout << "[ Scheduler | STARTED ]" << std::endl;
    }

    auto stop() -> void {
        watcher.isRunning = false;
        std::cout << "[ Scheduler | STOPPED ]" << std::endl;
    }

    auto restart() -> void {
        stop();
        start();
    }

    [[nodiscard]]
    auto is_running() const -> bool {
        return watcher.isRunning;
    }

    /////////////////////////

    auto get_runtime_info() -> std::string {
        const auto time_t = std::chrono::system_clock::to_time_t(up_time);
        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        const auto run_time = std::difftime(now, time_t);
        const auto days = std::floor(run_time / DAY_SECONDS);
        const auto hours = std::floor((run_time - (days * DAY_SECONDS)) / HOUR_SECONDS);
        const auto minutes = std::floor((run_time - (days * DAY_SECONDS) - (hours * HOUR_SECONDS)) / MINUTE_SECONDS);
        const auto seconds = std::floor((run_time - (days * DAY_SECONDS) - (hours * HOUR_SECONDS) - (minutes * MINUTE_SECONDS)));

        auto ss = new std::stringstream();
        *ss << "[ SCHEDULER | RUNTIME INFO ]" << std::endl;
        *ss << "--------------------------------------------------------------------------------" << std::endl;
        *ss << "Running since       " << std::ctime(&time_t) << std::endl;
        *ss << "Current time:       " << std::ctime(&now) << std::endl;
        *ss << "Run time:           " << days << " days, " << hours << " hours, "
            << minutes << " minutes and " << seconds << " seconds." << std::endl;
        *ss << "Scheduler contains  " << watchables.size() << " watchable(s)." << std::endl;
        *ss << "Time Table has      " << timeTable_ptr->size() << " entries." << std::endl;
        *ss << "Scheduler is        " << (watcher.isRunning ? "running" : "stopped") << "." << std::endl;
        *ss << "--------------------------------------------------------------------------------" << std::endl;

        std::string result = ss->str();
        delete ss;
        return result;
    }

protected:
    /// Map Key
    constexpr static std::string_view SCHEDULER = "Scheduler";

    /// Configuration attributes
    constexpr static std::string_view ENABLE_BLACKLISTS = "Enable Blacklists";

private:
    std::chrono::system_clock::time_point up_time = {};
    Dispatcher dispatcher;
    bool enable_blacklists = Dispatcher::to_bool_or_else(
            dispatcher.env_lookup(SCHEDULER, ENABLE_BLACKLISTS), true);
    std::vector<std::shared_ptr<Watchable>> watchables;
    std::shared_ptr<TimeTable> timeTable_ptr;
    Watcher watcher;

    /// STATIC CONSTANTS
    static constexpr int DAY_SECONDS = 86'400;
    static constexpr int HOUR_SECONDS = 3'600;
    static constexpr int MINUTE_SECONDS = 60;

    Scheduler() {
        up_time = std::chrono::system_clock::now();
        timeTable_ptr = std::make_shared<TimeTable>();
        watchables.emplace_back(timeTable_ptr);
        // drop folder support
        timeTable_subscribe_listener();
        timeTable_drop_listener();
    };

    /// \brief <h3>Time Table subscribe listener</h3>
    /// \details This method is called when a new job is added to the time table.
    /// \note This listener is <b>only</b> active when the scheduler is running.
    auto timeTable_subscribe_listener() -> void {
        timeTable_ptr->on_subscribe([this](std::shared_ptr<I_TimedAction> const& job) {
            std::cout << "[ TimeTable | SUBSCRIBE ] -> '" << job->getName() << "' " << std::endl;

            if(job->get_execution_times().empty()) {
                std::cout << "[ TimeTable | WARNING ] -> '" << job->getName()
                          << "' has no execution times." << std::endl;

                timeTable_ptr->drop(job);
            }
            else {
                const auto execution_count = job->get_execution_times().size();
                const bool is_single_execution = execution_count == 1;

                if(is_single_execution) {
                    std::cout << "[ TimeTable | INFO ] -> '" << job->getName()
                              << "' will be executed at " << std::asctime(&job->get_execution_times()[0]) << '.' << std::endl;
                }
                else {
                    std::cout << "[ TimeTable | INFO ] -> '" << job->getName()
                              << "' will be executed at " << std::asctime(&job->get_execution_times()[0]) << " and "
                              << execution_count - 1 << " more times." << std::endl;
                }
            }
        });
    }

    /// \brief <h3>Time Table drop listener</h3>
    /// \details This listener is called when a job is dropped from the timeTable.
    /// \note This listener is <b>only</b> active when the scheduler is running.
    auto timeTable_drop_listener() -> void {
        timeTable_ptr->on_listen([](std::shared_ptr<I_TimedAction> const& job) {
            std::cout << "[ TimeTable | DROPPED ] -> '" << job->getName() << "'" << std::endl;
        });
    }
};
