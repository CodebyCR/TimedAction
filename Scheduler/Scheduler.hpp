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
#include "JobMap.hpp"


class Scheduler {

private:
    std::shared_ptr<EventQueue> eventQueue_ptr;
    std::shared_ptr<JobMap> jobMap_ptr;
    Watcher watcher;
    // JobManager jobManager;


    Scheduler(){
        eventQueue_ptr = std::make_shared<EventQueue>();

        eventQueue_ptr->on_subscribe([](I_TimedAction* action) {
            std::cout << "EventQueue: subscribed to " << action->getName() << std::endl;
        });

        eventQueue_ptr->on_listen([](I_TimedAction* action) {
            std::cout << "EventQueue: listened to " << action->getName() << std::endl;
        });
    };


public:
    virtual ~Scheduler() = default;


    // Singleton
    static Scheduler& get_instance() {
        static Scheduler instance;
        return instance;
    }

    Scheduler(Scheduler const&) = delete;
    void operator=(Scheduler const&) = delete;

    auto add(I_TimedAction* action) -> void {
        eventQueue_ptr->push(action);
    }

    /// TODO: start & close new threads from the scheduler instated of inside the action
    // ! Refactor: This starts all eventQueue_ptr on separate threads
    // * What you want -> start the thread of the action if it is required
    auto start() const -> void {

        // load jobs

        // ? map mit <execution time, object ptr> -> sort by execution time

        // push jobs to eventQueue_ptr

        /// new watcher thread & make it independent
        auto _watcher_thread = watcher.getThread(jobMap_ptr);
        _watcher_thread.detach();

//        for (auto action : eventQueue_ptr) {
//            action->start();
//        }
    }

    auto stop() const -> void {
        for (auto &action : *eventQueue_ptr) {
            action->stop();
        }
    }

    auto restart() const -> void {
        for (auto &action : *eventQueue_ptr) {
            action->restart();
        }
    }

    [[nodiscard]]
    auto is_running() const -> bool {
        std::ranges::any_of(*eventQueue_ptr, [](auto &action) {
            return action->is_running();
        });
        return false;
    }

    /////////////////////////

    auto start_scheduler() -> void {
        auto job_list = std::map<std::string, std::tm>{};

        for(auto &action : *eventQueue_ptr) {
            const auto time_points = action->get_execution_times();
            for(auto &time_point : time_points) {
                job_list.emplace(action->getName(), time_point);
            }
        }
    }

};
