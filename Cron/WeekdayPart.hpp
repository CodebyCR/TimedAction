//
// Created by Christoph Rohde on 21.01.23.
//

#pragma once

#include <iostream>
#include <vector>
#include "../StringUtils.hpp"

class WeekdayPart {

private:
    std::string rawValue;
    std::vector<std::string> containedWeekdays;

    std::vector<std::string> weekdays = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};


    /**
     * Convert the Input (int 0-7 or string sun-Sat) to an Weekday short name in uppercase.
     * @param weekday
     * @return a Uppercase Weekday name (SUN, MON, TUE, WED, THU, FRI, SAT)
     */
    [[nodiscard]]
    auto to_weekday(std::string & weekday) const -> std::string {
        if(StringUtils::is_number(weekday)) {
            return weekdays[std::stoi(weekday)];
        }

        StringUtils::to_upper(weekday);
        return weekday;
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
            std::copy(weekdays.begin(), weekdays.end(), std::back_inserter(containedWeekdays));
            containedWeekdays.pop_back();
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

    WeekdayPart( const std::string &weekdays):
            rawValue(weekdays) {
        process();
    }

    ~WeekdayPart() = default;

//    auto operator << (std::ostream & os) const -> std::ostream & {
//        os << "Weekdays: ";
//        for (auto & weekday : containedWeekdays) {
//            os << weekday << " ";
//        }
//        return os;
//    }

    [[nodiscard]]
    auto contains(std::string_view const& weekday) const -> bool {
        for (auto const& containedWeekday : containedWeekdays) {
            if (containedWeekday == weekday) {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]]
    auto getContainedWeekdays() const -> std::vector<std::string> const& {
        return containedWeekdays;
    }

};