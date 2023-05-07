//
// Created by Christoph Rohde on 29.01.23.
//

#pragma once

#include <iostream>
#include <vector>

namespace Sort {

    /**
     * Sort the time points by the next reached time.
     * @param times
     */
    static auto by_next_reached_time(std::vector<std::tm>& times) {
        std::sort(times.begin(), times.end(), [](std::tm& a, std::tm& b) {
            const time_t timeA = std::mktime(&a);
            const time_t timeB = std::mktime(&b);
            return difftime(timeA, timeB) < 0;
        });
    }
}    // namespace Sort
