//
// Created by Christoph Rohde on 23.04.23.
//

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include "../TimedAction_Types/I_TimedAction.hpp"

class JobMap: public std::vector<std::pair<std::time_t, I_TimedAction*>> {
private:

public:

    JobMap() = default;
    JobMap(const JobMap &) = default;
    JobMap &operator=(const JobMap &) = default;


    /** sorts this vector by the key of the pair entries */
    auto sort() -> void {
        std::sort(this->begin(), this->end(), [](auto &left, auto &right) {
            return &left.first < &right.first;
        });
    }

    auto put(std::shared_ptr<I_TimedAction> const& action) -> void {

        auto executionTimes = action->get_execution_times();

        for (auto & executionTime : executionTimes) {
            std::cout << "JobMap: put " << action->getName() << " for execution at " << std::asctime(&executionTime) << std::endl;

            const auto time_t = std::mktime(&executionTime);
            this->push_back(std::make_pair(time_t, action.get()));
        }

        this->sort();
    }

    /** return all actions that are scheduled for execution at the given time */
    auto get(std::time_t const& time) -> std::vector<I_TimedAction*> {
        std::vector<I_TimedAction*> result;

        auto result_vec = std::ranges::filter_view(*this, [&](auto &pair) {
            return pair.first == time;
        });

        for (auto &pair : result_vec) {
            result.emplace_back(pair.second);
        }

        return result;
    }

    auto remove(std::time_t const& time) -> void {
        auto it = std::find_if(this->begin(), this->end(), [&](auto &pair) {
            return pair.first == time;
        });

        if (it != this->end()) {
            this->erase(it);
        }
    }

};
