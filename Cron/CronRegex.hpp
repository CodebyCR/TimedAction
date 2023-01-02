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

//    const std::string VALID_SECOND = R"(^([0-5]?[0-9]|\*)(-([0-5]?[0-9]))?(/([0-9]))?(,([0-5]?[0-9])(-([0-5]?[0-9]))?(/([0-9]))?)*$)";
//    const auto SECOND_REGEX = std::regex(VALID_SECOND);
//
//    const std::string  VALID_MINUTE = R"(^([0-5]?[0-9]|\*)(-([0-5]?[0-9]))?(/([0-9]))?(,([0-5]?[0-9])(-([0-5]?[0-9]))?(/([0-9]))?)*$)";
//    const auto MINUTE_REGEX = std::regex(VALID_MINUTE);
//
//    const std::string  VALID_HOUR = R"(^((\*)|(((([0-1]?[0-9])|([2][0-3])|([0-9]))(-(([0-1]?[0-9])|([2][0-3])|([0-9])))?)(,((([0-1]?[0-9])|([2][0-3])|([0-9]))(-(([0-1]?[0-9])|([2][0-3])|([0-9])))?))*)))$)";
//    const auto HOUR_REGEX = std::regex(VALID_HOUR);
//
//    const std::string  VALID_DAY_OF_MONTH = R"(^((\*)|(((([1-2]?[0-9])|([3][0-1])|([0-9]))(-(([1-2]?[0-9])|([3][0-1])|([0-9])))?)(,((([1-2]?[0-9])|([3][0-1])|([0-9]))(-(([1-2]?[0-9])|([3][0-1])|([0-9])))?))*)))$)";
//    // R"(^([1-9]|[12][0-9]|3[01]|\*)(-([1-9]|[12][0-9]|3[01]))?(/([1-9][0-9]*))?(,([1-9]|[12][0-9]|3[01])(-([1-9]|[12][0-9]|3[01]))?(/([1-9][0-9]*))?)*$)"
//    const auto DAY_OF_MONTH_REGEX = std::regex(VALID_DAY_OF_MONTH);
//
//    const std::string  VALID_MONTH = R"(^((\*)|(((([1-9])|([1][0-2])|([0-9]))(-(([1-9])|([1][0-2])|([0-9])))?)(,((([1-9])|([1][0-2])|([0-9]))(-(([1-9])|([1][0-2])|([0-9])))?))*)))$)";
//    // R"(^((\*)|(((([1-9])|([1][0-2]))(-(([1-9])|([1][0-2])))?)(,((([1-9])|([1][0-2]))(-(([1-9])|([1][0-2])))?))*)))$)")
//    const auto MONTH_REGEX = std::regex(VALID_MONTH);
//
//    const std::string  VALID_DAY_OF_WEEK = R"(^((\*)|(((([0-6])|(SUN|MON|TUE|WED|THU|FRI|SAT))(-(([0-6])|(SUN|MON|TUE|WED|THU|FRI|SAT)))?)(,((([0-6])|(SUN|MON|TUE|WED|THU|FRI|SAT))(-(([0-6])|(SUN|MON|TUE|WED|THU|FRI|SAT)))?))*)))$)";
//    const auto DAY_OF_WEEK_REGEX = std::regex(VALID_DAY_OF_WEEK);
//
//    const std::string  VALID_YEAR = R"(^((\*)|(((([1-9][0-9][0-9][0-9])|([0-9][0-9][0-9][0-9]))(-(([1-9][0-9][0-9][0-9])|([0-9][0-9][0-9][0-9])))?)(,((([1-9][0-9][0-9][0-9])|([0-9][0-9][0-9][0-9]))(-(([1-9][0-9][0-9][0-9])|([0-9][0-9][0-9][0-9])))?))*)))$)";
//    // R"(^((\*)|(((([1970-2099])|(\*\*))(-(([1970-2099])|(\*\*)))?)(,((([1970-2099])|(\*\*))(-(([1970-2099])|(\*\*)))?))*)))$)"
//    const auto YEAR_REGEX = std::regex(VALID_YEAR);


    [[maybe_unused]]
    static auto isValidCron(std::vector<std::string> &cronParts) -> bool {

        if (cronParts.size() < 6) {
            std::cerr << "Cron expression must have at least 6 parts" << std::endl;
            return false;
        }

        if (cronParts.size() == 6) {
            // without seconds
            cronParts.emplace(cronParts.begin(), "0");
        }


//        if (constexpr auto SECOND = 0; !std::regex_match(cronParts.at(SECOND), SECOND_REGEX)) {
//            std::cerr << "Invalid second: " << cronParts.at(SECOND) << std::endl;
//            return false;
//        }
//
//        if (constexpr auto MINUTE = 1; !std::regex_match(cronParts.at(MINUTE), MINUTE_REGEX)) {
//            std::cerr << "Invalid minute: " << cronParts.at(MINUTE) << std::endl;
//            return false;
//        }
//
//        if (constexpr auto HOUR = 2; !std::regex_match(cronParts.at(HOUR), HOUR_REGEX)) {
//            std::cerr << "Invalid hour: " << cronParts.at(HOUR) << std::endl;
//            return false;
//        }
//
//        if (constexpr auto DAY_OF_MONTH = 3; !std::regex_match(cronParts.at(DAY_OF_MONTH), DAY_OF_MONTH_REGEX)) {
//            std::cerr << "Invalid day of month: " << cronParts.at(DAY_OF_MONTH) << std::endl;
//            return false;
//        }
//
//        if (constexpr auto MONTH = 4; !std::regex_match(cronParts.at(MONTH), MONTH_REGEX)) {
//            std::cerr << "Invalid month: " << cronParts.at(MONTH) << std::endl;
//            return false;
//        }
//
//        if (constexpr auto DAY_OF_WEEK = 5; !std::regex_match(cronParts.at(DAY_OF_WEEK), DAY_OF_WEEK_REGEX)) {
//            std::cerr << "Invalid day of week: " << cronParts.at(DAY_OF_WEEK) << std::endl;
//            return false;
//        }
//
//        if (constexpr auto YEAR = 6; !std::regex_match(cronParts.at(YEAR), YEAR_REGEX)) {
//            std::cerr << "Invalid year: " << cronParts.at(YEAR) << std::endl;
//            return false;
//        }

        return true;
    }


}

