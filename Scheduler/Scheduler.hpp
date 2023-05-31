//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include "../Cron/CronInterpreter.hpp"
#include "../TimedAction_Types/I_TimedAction.hpp"
#include "EventQueue.hpp"
#include "JobManager.hpp"
#include "TimeTable.hpp"
#include "Watcher.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>


class Scheduler {
private:
    //    std::shared_ptr<EventQueue> eventQueue_ptr;
    std::map<std::string_view, std::string_view> attributes;
    std::shared_ptr<TimeTable> timeTable_ptr;
    Watcher watcher = Watcher();


    Scheduler() {
        attributes = {};
        timeTable_ptr = std::make_shared<TimeTable>();
        // drop folder support
        timeTable_subscribe_listener();
        timeTable_drop_listener();
    };

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
