//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../TimedAction_Types/I_TimedAction.hpp"
#include "../Cron/CronInterpreter.hpp"
#include "EventQueue.hpp"
#include <map>


class Scheduler {

private:
    EventQueue actions;

    Scheduler(){
        actions.subscribe([](I_TimedAction* action) {
            std::cout << "EventQueue: subscribed to " << action->getName() << std::endl;
        });

        actions.listen( [](I_TimedAction* action) {
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
        actions.push(action);
    }

    /// TODO: start & close new threads from the scheduler instated of inside the action
    // ! Refactor: This starts all actions on separate threads
    // * What you want -> start the thread of the action if it is required
    auto start() const -> void {
        for (auto action : actions) {
            action->start();
        }
    }

    auto stop() const -> void {
        for (auto &action : actions) {
            action->stop();
        }
    }

    auto restart() const -> void {
        for (auto &action : actions) {
            action->restart();
        }
    }

    [[nodiscard]]
    auto is_running() const -> bool {
        std::ranges::any_of(actions, [](auto &action) {
            return action->is_running();
        });
        return false;
    }

    /////////////////////////

    auto start_scheduler() -> void {
        auto job_list = std::map<std::string, std::tm>{};

        for(auto &action : actions) {
            const auto time_points = action->get_execution_times();
            for(auto &time_point : time_points) {
                job_list.emplace(action->getName(), time_point);
            }
        }
    }

};
