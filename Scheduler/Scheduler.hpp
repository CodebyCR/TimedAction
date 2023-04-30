//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "../TimedAction_Types/I_TimedAction.hpp"
#include "../Cron/CronInterpreter.hpp"
#include "EventQueue.hpp"
#include "Watcher.hpp"
#include "JobManager.hpp"
#include "TimeTable.hpp"


class Scheduler {

private:
//    std::shared_ptr<EventQueue> eventQueue_ptr;
    std::shared_ptr<TimeTable>  timeTable_ptr;
    Watcher watcher = Watcher();



    // JobManager jobManager;


    Scheduler(){

        timeTable_ptr = std::make_shared<TimeTable>();

//        eventQueue_ptr = std::make_shared<EventQueue>();


        timeTable_ptr->on_subscribe([](std::pair<std::time_t, I_TimedAction*> entry) {
            std::cout << "TimeTable: subscribed to " << entry.second->getName() << std::endl;
        });

        timeTable_ptr->on_listen([](std::pair<std::time_t, I_TimedAction*> entry) {
            std::cout << "TimeTable: listened to " << entry.second->getName() << std::endl;
            // TODO: count down the execution times for the action and remove it from the timeTable_ptr if it is 0
            // "TimeTable: dropped " << entry.second->getName() << std::endl;
            // remove for less entries
        });

    };


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

    /// TODO: start & close new threads from the scheduler instated of inside the action
    // ! Refactor: This starts all eventQueue_ptr on separate threads
    // * What you want -> start the thread of the action if it is required
    auto start() -> void {

        /// new watcher thread & make it independent
        auto watcher_thread = watcher.getThread(timeTable_ptr);
        watcher_thread.detach();

    }

    auto stop() -> void {
        watcher.isRunning = false;

        for (auto &entry : *timeTable_ptr) {
            entry.second->stop();
        }
    }

    auto restart() const -> void {
//        for (auto &entry : *timeTable_ptr) {
//            entry.second->restart();
//        }
        std::cout << "Scheduler::restart() not implemented" << std::endl;
    }

    [[nodiscard]]
    auto is_running() const -> bool {
        std::ranges::any_of(*timeTable_ptr, [](auto &entry) {
            return entry.second->is_running();
        });
        return false;
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

