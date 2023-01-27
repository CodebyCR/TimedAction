//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../TimedAction_Types/I_TimedAction.hpp"
#include "../Cron/CronInterpreter.hpp"
#include <map>

class Scheduler {

private:

    Scheduler() = default;

    std::vector<I_TimedAction*> actions;


public:

    // Singleton
    static Scheduler& get_instance() {
        static Scheduler instance;
        return instance;
    }

    Scheduler(Scheduler const&) = default;
    void operator=(Scheduler const&) = delete;

    auto add(I_TimedAction* action) -> void {
        actions.push_back(action);
    }

    auto start() const -> void {
        for (auto &action : actions) {
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
        for (auto &action : actions) {
            if (action->is_running()) {
                return true;
            }
        }
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
