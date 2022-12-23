//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "I_TimedAction.hpp"

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





};
