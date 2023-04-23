//
// Created by Christoph Rohde on 23.04.23.
//

#pragma once

#include <iostream>
#include <map>
#include <sort>
#include "../TimedAction_Types/I_TimedAction.hpp"

class JobMap : public std::vector<std::pair<std::tm, I_TimedAction*>> {
private:

public:

    JobMap() = default;
    JobMap(const JobMap &) = default;
    JobMap &operator=(const JobMap &) = default;


    auto put(std::shared_ptr<I_TimedAction> const& action) -> void {

        auto executionTimes = action->get_execution_times();

        for (auto & executionTime : executionTimes) {
            std::cout << "JobMap: put " << action->getName() << " for execution at " << std::asctime(&executionTime) << std::endl;
            this->push_back(std::make_pair(executionTime, action.get()));
        }

    }

    auto sort() -> void {
        std::sort(this->begin(), this->end(), [](auto const& a, auto const& b) {
            return a.first < b.first;
        });
    }


};
