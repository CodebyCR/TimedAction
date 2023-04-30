//
// Created by Christoph Rohde on 29.04.23.
//

#pragma once

#include <iostream>
#include "../TimedAction_Types/I_TimedAction.hpp"
#include <sstream>
#include <ranges>


class TimeTable: public std::vector<std::pair<std::time_t, I_TimedAction*>>  {
    // * REFACTOR: use std::multimap instead of std::vector
    // use std::map<std::time_t, std::vector<I_TimedAction*>> instead of std::vector<std::pair<std::time_t, I_TimedAction*>>
private:
    std::function<void(std::pair<std::time_t, I_TimedAction*>)> _subscribe;
    std::function<void(std::pair<std::time_t, I_TimedAction*>)> _listen;


public:

    auto add(I_TimedAction* const& action) -> void
    {
        auto executionTimes = action->get_execution_times();


        std::cout << action->execution_time_count_message() << std::endl;

        for (auto & executionTime : executionTimes) {
            const auto time_t = std::mktime(&executionTime);
            this->push_back(std::make_pair(time_t, action));
        }

        if(_subscribe) {
            _subscribe(std::make_pair(0, action));
        }

        //this->sort();

    }

    auto get(std::time_t const& time) -> std::vector<I_TimedAction*>
    {
        std::vector<I_TimedAction*> result;

        auto result_vec = std::ranges::filter_view(*this, [&](auto &pair) {
            return pair.first == time;
        });

        for (auto &pair : result_vec) {
            result.emplace_back(pair.second);
        }

        return result;
    }

    /// Sort

    auto sort() -> void
    {
        std::sort(this->begin(), this->end(), [](auto &left, auto &right) {
            return &left.first < &right.first;
        });
    }


    /// Listener

    auto on_listen(std::function<void(std::pair<std::time_t, I_TimedAction*>)> listen) -> void
    {
        this->_listen = std::move(listen);
    }

    auto on_subscribe(std::function<void(std::pair<std::time_t, I_TimedAction*>)> subscribe) -> void
    {
        this->_subscribe = std::move(subscribe);
    }

};
