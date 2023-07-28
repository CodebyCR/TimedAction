//
// Created by Christoph Rohde on 29.04.23.
//

#pragma once

#include "../Interfaces/I_TimedAction.hpp"
#include <iostream>
#include <map>
#include <ranges>
#include <sstream>
#include "../Interfaces/Watchable.hpp"


class TimeTable : public Watchable,
                  public std::map<std::time_t, std::vector<std::shared_ptr<I_TimedAction>>>{
private:
    std::function<void(std::shared_ptr<I_TimedAction>)> _subscribe;
    std::function<void(std::shared_ptr<I_TimedAction>)> _listen;


public:
    auto add(std::shared_ptr<I_TimedAction> const& action) -> void {
        if(_subscribe) {
            _subscribe(action);
        }

        auto executionTimes = action->get_execution_times();

        for(auto& executionTime: executionTimes) {
            const auto time_t = std::mktime(&executionTime);

            if(this->find(time_t) == this->end()) {
                this->insert(std::make_pair(time_t, std::vector<std::shared_ptr<I_TimedAction>>()));
            }

            this->at(time_t).emplace_back(action);
        }

    }

    auto get(std::time_t const& time) -> std::vector<std::shared_ptr<I_TimedAction>> {
        if(this->find(time) == this->end()) {
            return {};
        }

        return this->at(time);
    }

    /// Erased all entries of the action from the timetable.
    auto drop(std::shared_ptr<I_TimedAction> const& action) -> void {
        for(auto& entry: *this) {
            auto& actions = entry.second;

            actions.erase(std::remove(actions.begin(), actions.end(), action), actions.end());

            if(actions.empty()) {
                this->erase(entry.first);
            }
        }

        if(_listen) {
            _listen(action);
        }
    }

    auto remove(std::time_t const& time) -> void {
        this->erase(time);
    }

    /// This methode checks the given list of actions for finished tasks.
    /// <br/>If a task is finished, it will be removed from the timetable.
    auto check_status(std::vector<std::shared_ptr<I_TimedAction>>& actions, const std::time_t& now) -> void {
        for (auto& action : actions) {
            std::future<Message> task = action->finished();

            switch(const auto result = task.wait_for(std::chrono::milliseconds(10)); result) {
                case std::future_status::ready: {
                    std::cout << "[ TimeTable ] -> finished " << action->getName() << std::endl;
                    // start remove execution time from job here...
                    this->remove(now);
                } break;

                case std::future_status::deferred: {
                    std::cout << "[ TimeTable ] -> deferred for " << action->getName() << std::endl;
                } break;

                case std::future_status::timeout: {
                    std::cout << "[ TimeTable ] -> timeout for " << action->getName() << std::endl;
                    this->remove(now);
                } break;

                default: {
                    std::cout << "[ TimeTable | ERROR ] -> unknown status of " << action->getName() << std::endl;
                }
            }
        }
    }


    /// Listener

    auto on_listen(std::function<void(std::shared_ptr<I_TimedAction>)> listen) -> void {
        this->_listen = std::move(listen);
    }

    auto on_subscribe(std::function<void(std::shared_ptr<I_TimedAction>)> subscribe) -> void {
        this->_subscribe = std::move(subscribe);
    }

    auto watch(std::time_t const& now) -> void override {
        auto time_t_vec = this->get(now);

//        std::cout << "[ Watcher | CHECKING ] -> " << time_t_vec.size() << " found." << std::endl;

        /// check if jobs for execution
        std::ranges::for_each(time_t_vec, [&](std::shared_ptr<I_TimedAction>& time_t) {
            const auto asc_t = std::asctime(std::localtime(&now));
            std::cout << "[ TimeTable | FOUND ] -> " << time_t->getName() << " for execution at " << asc_t << std::endl;
            time_t->start();
        });

        /// check if jobs for finished execution
        this->check_status(time_t_vec, now);
    }

    auto inactive() -> bool override {
        return this->empty();
    }

};
