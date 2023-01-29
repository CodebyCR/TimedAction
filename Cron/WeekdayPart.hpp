//
// Created by Christoph Rohde on 21.01.23.
//

#pragma once

#include <iostream>
#include <vector>
#include "../StringUtils.hpp"

class WeekdayPart {

private:
    std::string name = "weekday";
    std::string rawValue;
    std::vector<int> containedWeekdays;

    std::vector<std::string> weekdays = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};


    /**
     * Convert the Input (int 0-7 or string sun-Sat) to an Weekday short name in uppercase.
     * @param weekday
     * @return a int value between 0-6
     */
    [[nodiscard]]
    auto to_weekday(std::string & weekday) const -> int {
        if(StringUtils::is_number(weekday)) {
            return std::stoi(weekday);
        }

        StringUtils::to_upper(weekday);

        for(short index = 0; index < weekdays.size(); index++) {
            if(weekdays[index] == weekday) {
                return index;
            }
        }

        return -1;
    }

    auto add_raw_values(std::vector<std::string> & raw_values) -> void {
        for( auto & currentWeekday : raw_values) {
            const auto weekday = to_weekday(currentWeekday);
            containedWeekdays.push_back(weekday);
        }
    }

    auto list() -> void {
        auto raw_list = StringUtils::split_by(rawValue, ',');
        add_raw_values(raw_list);
    }

    auto range() -> void {
        auto raw_range = StringUtils::split_by(rawValue, '-');
        add_raw_values(raw_range);
    }

    void process() {
        if (rawValue == "*") {
            containedWeekdays = {0, 1, 2, 3, 4, 5, 6};
            return;
        }

        if(const bool isList = StringUtils::contains(rawValue, ",")){
            list();
            return;
        }

        if(const bool isRange = StringUtils::contains(rawValue, "-")){
            range();
            return;
        }

        // Single value
        containedWeekdays.push_back(to_weekday(rawValue));
    }

public:

    WeekdayPart() = default;

    WeekdayPart(std::string const& weekdays):
            rawValue(weekdays) {
        process();
    }

    ~WeekdayPart() = default;

    [[nodiscard]]
    auto getName() const -> std::string {
        return name;
    }

    [[nodiscard]]
    auto getContainedWeekdays() const -> std::vector<int> const& {
        return containedWeekdays;
    }

};