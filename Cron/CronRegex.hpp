//
// Created by Christoph Rohde on 20.12.22.
//

#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>

namespace CronRegex {

    const auto SECOND_REGEX = std::regex(R"(^\s*(\*|[0-5]?\d)(/[0-5]?\d)?\s+)");
    const auto MINUTE_REGEX  = std::regex(R"(^\s*(\*|[0-5]?\d)(/[0-5]?\d)?\s+)");
    const auto HOUR_REGEX  = std::regex(R"(^\s*(\*|[0-5]?\d)(/[0-5]?\d)?\s+)");
    const auto DAY_OF_MONTH_REGEX  = std::regex(R"(^\s*(\*|[0-5]?\d)(/[0-5]?\d)?\s+)");
    const auto MONTH_REGEX  = std::regex(R"(^\s*(\*|[0-5]?\d)(/[0-5]?\d)?\s+)");
    const auto DAY_OF_WEEK_REGEX  = std::regex(R"(^\s*(\*|[0-9]|[0-5][0-9]|MON|TUE|WED|THU|FRI|SAT|SUN)(-MON|-TUE|-WED|-THU|-FRI|-SAT|-SUN)?\s*$)");

    auto split(const std::string& str, char delimiter) -> std::vector<std::string> {
        std::vector<std::string> res;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            res.push_back(item);
        }
        return res;
    }

    [[maybe_unused]]
    static auto isCronRegex(const std::string& cronString) {

        if(cronString.empty()) {
            return false;
        }

        auto cronParts = split(cronString, ' ');

        if(cronParts.size() < 5) {
            return false;
        }

        if (cronParts.size() == 5){
            // without seconds
            cronParts.emplace(cronParts.begin(), "0");
        }


        if(constexpr auto SECOND = 0; std::regex_match(cronParts.at(SECOND), SECOND_REGEX )) {
            return true;
        }

        if(constexpr auto MINUTE = 1; std::regex_match(cronParts.at(MINUTE), MINUTE_REGEX )) {
            return true;
        }

        if(constexpr auto HOUR = 2; std::regex_match(cronParts.at(HOUR), HOUR_REGEX )) {
            return true;
        }

        if(constexpr auto DAY_OF_MONTH = 3; std::regex_match(cronParts.at(DAY_OF_MONTH), DAY_OF_MONTH_REGEX )){
            return true;
        }

        if(constexpr auto MONTH = 4; std::regex_match(cronParts.at(MONTH), MONTH_REGEX )) {
            return true;
        }

        if(constexpr auto DAY_OF_WEEK = 5; std::regex_match(cronParts.at(DAY_OF_WEEK), DAY_OF_WEEK_REGEX )) {
            return true;
        }



    }


}

