//
// Created by Christoph Rohde on 09.06.23.
//

#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace CronRanges {

    bool is_number(const std::string& str) {
        bool is_number = std::ranges::all_of(str, [](char c) { return std::isdigit(c); });

        return is_number;
    }

    auto to_upper(const std::string& str) -> std::string {
        std::string upperStr = str;
        std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
        return upperStr;
    }

    auto split(const std::string& str, char delimiter) -> std::vector<std::string> {
        auto result = std::vector<std::string>();
        auto current = std::string();
        for (auto c : str) {
            if (c == delimiter) {
                result.push_back(current);
                current.clear();
            }
            else {
                current += c;
            }
        }
        result.push_back(current);
        return result;
    }


    struct CronRanges {
        unsigned short start;
        unsigned short end;

        auto static get_index(const std::string& value) -> int {
            if(value == "SUN"){
                return 0;
            }
            else if(value == "MON"){
                return 1;
            }
            else if(value == "TUE"){
                return 2;
            }
            else if(value == "WED"){
                return 3;
            }
            else if(value == "THU"){
                return 4;
            }
            else if(value == "FRI"){
                return 5;
            }
            else if(value == "SAT"){
                return 6;
            }
            else{
                return 0;
            }
        }


        explicit CronRanges(const std::string& range) {
            auto rangeValues = split(range, '-');

            if(is_number(rangeValues[0])){
                this->start = std::stoi(rangeValues[0]);
            }
            else{
                this->start = get_index(to_upper(rangeValues[0]));
            }

            if(is_number(rangeValues[1])){
                this->end = std::stoi(rangeValues[1]);
            }
            else{
                this->end = get_index(to_upper(rangeValues[1]));
            }

        }
    };

    auto static range_matched(const std::string& range, const std::regex& regex ) -> bool {
        if(!range.contains('-')){
            return false;
        }

        if (CronRanges rangeValues(range); rangeValues.start >= rangeValues.end) {
            return false;
        }

        return std::regex_match(range, regex);
    }

    auto static second_range_matched(const std::string& range) -> bool {
        const static std::string second_range = "^(0?[0-9]|[1-5][0-9])(-?(?!0)(0?[0-9]|[1-5][0-9]))?$";
        std::regex second_range_regex(second_range);

        if(!range.contains('-')){
            return false;
        }

        if (CronRanges rangeValues(range); rangeValues.start >= rangeValues.end) {
            return false;
        }

        return std::regex_match(range, second_range_regex);
    }

    auto static hour_range_matched(const std::string& range) -> bool {
        const static std::string hour_range = "^(0?[0-9]|1[0-9]|2[0-3])(-?(?!0)(0?[0-9]|1[0-9]|2[0-3]))?$";
        std::regex hour_range_regex(hour_range);

        if(!range.contains('-')){
            return false;
        }

        if (CronRanges rangeValues(range); rangeValues.start >= rangeValues.end) {
            return false;
        }

        return std::regex_match(range, hour_range_regex);
    }

    auto day_range_matched(const std::string& range) -> bool {
        const static std::string day_range = "^(0?[1-9]|[12][0-9]|3[01])(-?(?!0)(0?[1-9]|[12][0-9]|3[01]))?$";
        std::regex day_range_regex(day_range);

        if(!range.contains('-')){
            return false;
        }

        if (CronRanges rangeValues(range); rangeValues.start >= rangeValues.end) {
            return false;
        }

        return std::regex_match(range, day_range_regex);
    }

    auto month_range_matched(const std::string& range) -> bool {
        const static std::string month_range = "^(0?[1-9]|1[0-2])(-?(?!0)(0?[1-9]|1[0-2]))?$";
        std::regex month_range_regex(month_range);

        if(!range.contains('-')){
            return false;
        }

        if (CronRanges rangeValues(range); rangeValues.start >= rangeValues.end) {
            return false;
        }

        return std::regex_match(range, month_range_regex);
    }

    auto weekday_range_matched(const std::string& range) -> bool {
        const static std::string weekday_range = "^(0?[0-7])(-?(?!0)(0?[0-7]))?$";
        std::regex weekday_range_regex(weekday_range);

        if(!range.contains('-')){
            return false;
        }

        if (CronRanges rangeValues(range); rangeValues.start >= rangeValues.end) {
            return false;
        }


        return std::regex_match(range, weekday_range_regex);
    }
}
