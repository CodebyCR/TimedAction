//
// Created by Christoph Rohde on 29.04.23.
//

#pragma once

#include <iostream>
#include "../TimedAction_Types/I_TimedAction.hpp"
#include <sstream>
#include <ranges>


class TimeTable: public std::map<std::time_t, std::vector<I_TimedAction*>>  {
private:
    std::function<void(I_TimedAction*)> _subscribe;
    std::function<void(I_TimedAction*)> _listen;


public:

    auto add(I_TimedAction* const& action) -> void
    {
        if(_subscribe) {
            _subscribe(action);
        }

        auto executionTimes = action->get_execution_times();

        for (auto & executionTime : executionTimes) {
            const auto time_t = std::mktime(&executionTime);

            if (this->find(time_t) == this->end()) {
                this->insert(std::make_pair(time_t, std::vector<I_TimedAction*>()));
            }

            this->at(time_t).emplace_back(action);

        }



        //this->sort();

    }

    auto get(std::time_t const& time) -> std::vector<I_TimedAction*>
    {
        if (this->find(time) == this->end()) {
            return {};
        }

        return this->at(time);
    }

    auto drop(I_TimedAction* action) -> void
    {
        // Erase all entries with action
        for (auto & entry : *this) {
            auto & actions = entry.second;

            actions.erase(std::remove(actions.begin(), actions.end(), action), actions.end());

            if (actions.empty()) {
                this->erase(entry.first);
            }
        }

        if(_listen) {
            _listen(action);
        }
    }




    /// Listener

    auto on_listen(std::function<void(I_TimedAction*)> listen) -> void
    {
        this->_listen = std::move(listen);
    }

    auto on_subscribe(std::function<void(I_TimedAction*)> subscribe) -> void
    {
        this->_subscribe = std::move(subscribe);
    }

    // On empty  for watcher standby mode

};
