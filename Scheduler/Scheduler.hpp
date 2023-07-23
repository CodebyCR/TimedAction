//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include "../Cron/CronInterpreter.hpp"
#include "../Interfaces/I_TimedAction.hpp"
#include "EventQueue.hpp"
#include "JobManager.hpp"
#include "TimeTable.hpp"
#include "Watcher.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <cmath>


class Scheduler {
private:
    std::chrono::system_clock::time_point up_time = {};
    //    std::shared_ptr<EventQueue> eventQueue_ptr;
    std::map<std::string_view, std::string_view> attributes;
    std::shared_ptr<TimeTable> timeTable_ptr;
    Watcher watcher = Watcher();


    Scheduler() {
        up_time = std::chrono::system_clock::now();
        attributes = {};
        timeTable_ptr = std::make_shared<TimeTable>();
        // drop folder support
        timeTable_subscribe_listener();
        timeTable_drop_listener();
    };

    /// \brief <h3>Time Table subscribe listener</h3>
    /// \details This method is called when a new job is added to the time table.
    /// \note This listener is <b>only</b> active when the scheduler is running.
    auto timeTable_subscribe_listener() -> void {
        timeTable_ptr->on_subscribe([this](I_TimedAction* job) {
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
                              << "' will be executed at " << std::asctime(&job->get_execution_times()[0]) << "." << std::endl;
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
        timeTable_ptr->on_listen([](I_TimedAction* job) {
            std::cout << "[ TimeTable | DROPPED ] -> '" << job->getName() << "'" << std::endl;
        });
    }


public:
    virtual ~Scheduler() = default;


    // Singleton
    static auto get_instance() -> Scheduler& {
        static Scheduler instance;
        return instance;
    }

    Scheduler(Scheduler const&) = default;
    void operator=(Scheduler const&) = delete;

    auto add(I_TimedAction* action) -> void {
        //eventQueue_ptr->push(action);
        timeTable_ptr->add(action);
    }

    auto start() -> void {
        /// new watcher thread & make it independent
        auto watcher_thread = watcher.getThread(timeTable_ptr);
        watcher_thread.detach();
    }

    auto stop() -> void {
        watcher.isRunning = false;
    }

    auto restart() const -> void {
        //        for (auto &entry : *timeTable_ptr) {
        //            entry.second->restart();
        //        }
        std::cout << "Scheduler::restart() not implemented" << std::endl;
    }

    [[nodiscard]] auto is_running() const -> bool {
        return watcher.isRunning;
    }

    auto get_attribute(std::string_view const& key) -> std::string_view {
        return this->attributes[key];
    }

    auto set_attribute(std::string_view const& key, std::string_view const& value) -> bool {
        if(key.empty() || value.empty()) {
            return false;
        }

        this->attributes[key] = value;
        return true;
    }

    /////////////////////////

    auto start_scheduler() -> void {
        //        auto job_list = std::map<std::string, std::tm>{};
        //
        //        for(auto &action : *eventQueue_ptr) {
        //            const auto time_points = action->get_execution_times();
        //            for(auto &time_point : time_points) {
        //                job_list.emplace(action->getName(), time_point);
        //            }
        //        }
        std::cout << "Scheduler::start_scheduler() not implemented" << std::endl;
    }

    auto get_runtime_info() -> std::string {
        const auto time_t = std::chrono::system_clock::to_time_t(up_time);
        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // run time in days + extra hours with minutes and seconds and
        const auto run_time = std::difftime(now, time_t);
        const auto days = std::floor(run_time / 86'400);
        const auto hours = std::floor((run_time - (days * 86'400)) / 3'600);
        const auto minutes = std::floor((run_time - (days * 86'400) - (hours * 3'600)) / 60);
        const auto seconds = std::floor((run_time - (days * 86'400) - (hours * 3'600) - (minutes * 60)));

        auto ss = new std::stringstream ();
        *ss << "[SCHEDULER RUNTIME INFO]" << std::endl;
        *ss << "--------------------------------------------------------------------------------" << std::endl;
        *ss << "Scheduler is running since " << std::ctime(&time_t) << std::endl;
        *ss << "Current time is " << std::ctime(&now) << std::endl;
        *ss << "Scheduler run time is " << days << " days, " << hours << " hours, " << minutes << " minutes and " << seconds << " seconds." << std::endl;
        *ss << "Scheduler has " << timeTable_ptr->size() << " jobs in the time table." << std::endl;
        *ss << "Scheduler is " << (watcher.isRunning ? "running" : "stopped") << "." << std::endl;
        *ss << "--------------------------------------------------------------------------------" << std::endl;

        return ss->str();
    }
};


/// example Scheduler implementation

// auto main() -> int {
//     auto scheduler = Scheduler::get_instance();
//     auto action = std::make_unique<TimedAction>("action", CronInterpreter::parse("*/5 * * * * *"));
//     scheduler.add(action.get());
//     scheduler.start();
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//     scheduler.stop();
//     return 0;
// }
