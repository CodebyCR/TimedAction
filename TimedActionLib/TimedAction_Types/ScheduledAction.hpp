//
// Created by Christoph Rohde on 24.07.23.
//
#pragma once

#include "../Interfaces/ActionCapsule.hpp"
#include "../Interfaces/I_TimedAction.hpp"
#include "../Notification/JobLog.hpp"
#include <chrono>
#include <iostream>
#include <thread>


class ScheduledAction : public I_TimedAction {
private:

    /** Internal state */

    bool isRunning {};
    std::thread thread;



public:
    ScheduledAction(std::string_view const& name,
                    I_ExecutionTimer* executionTimer,
                    std::function<void(void)> const& action)
        : I_TimedAction(name, executionTimer, action){}

    explicit ScheduledAction(ActionCapsule const& action)
        : I_TimedAction(action.name, action.execution_timer, action.action){}

    ~ScheduledAction() override = default;

    // copy and move constructors
    ScheduledAction(ScheduledAction const& other) = delete;
    ScheduledAction(ScheduledAction&& other) noexcept = default;
    auto operator=(ScheduledAction const& other) -> ScheduledAction& = delete;
    auto operator=(ScheduledAction&& other) noexcept -> ScheduledAction& = default;




    auto run() -> void {
        if (action) {
            action();
        }
        else{
            std::cout << "action is nullptr" << std::endl;
        }
    }

    auto start() -> void override {
        isRunning = true;
        thread = std::thread(&ScheduledAction::run, this);
        thread.detach();
    }

    auto stop() -> void override {
        isRunning = false;
        thread.join();
    }

//    auto restart() -> void override {
//        stop();
//        start();
//    }
//
//    [[nodiscard]]
//    auto is_running() const -> bool override {
//        return isRunning;
//    }

    //moved a shared_ptr of this


};


