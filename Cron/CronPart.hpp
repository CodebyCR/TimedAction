//
// Created by Christoph Rohde on 24.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <../StringUtils.hpp>
#include <forward_list>
#include "LeapYearUtils.hpp"
#include <ctime>


class CronPart {
private:

    std::string name;
    std::forward_list<std::chrono::seconds> times;
    std::string rawValue;

    bool isNumber;
    bool isRange;
    bool isList;
    bool isWildcard;
    bool isPeriodic;

    u_long multiplier;
    uint8_t range;


    /** 1-31 || * || *\/2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String */
    void processValue(const std::string &basicString) {

        if (isNumber) {
            numberValue(basicString);
        }

        if (this->isWildcard) {
            // *
            // basicString is wildcard
            if(this->name == "year"){
                auto current_year = LeapYearUtils::getCurrentYear();
                //this->value = current_year;
            }else{

                /// Range works
                for (int index = 0; index < this->range; index++) {
                    this->times.emplace_front((index + 1) * this->multiplier);
                }
            }
        }

        if (StringUtils::contains(basicString, "*") && basicString.length() > 1) {
            // */2
            // basicString is periodic
            stepValue(basicString);
        }

        if (this->isRange) {
            // 1-5
            // basicString is range
            rangeValue(basicString);
        }

        if (this->isList) {
            // 1,2,3
            // basicString is list
            listValue(basicString);
        }

        if (this->isPeriodic) {
            // 1-5/2
            periodValue(basicString);
        }

    }

    /**
     * @return multiplier (value of section in seconds) & range (max value of section)
     */
    [[nodiscard]]
    auto getPartRangeSize() const -> std::pair<u_long, uint8_t> {

        if (this->name == "second") {
            return std::make_pair(1, 60);
        }
        if (this->name == "minute") {
            return std::make_pair(60, 60);
        }
        if (this->name == "hour") {
            return std::make_pair(3'600, 24);
        }
        if (this->name == "day") {
            return std::make_pair(86'400, 31);
        }
        if (this->name == "month") {
            auto monthMultiplier = LeapYearUtils::getDaysInCurrentMonth();
            std::cout << "monthMultiplier: " << monthMultiplier << std::endl;

            // seconds for the specific month in the current year, multiplied by
            return std::make_pair(86'400 * monthMultiplier, 12);
        }
        if (this->name == "weekday") {

            // heraus finden  welcher Tag der Woche ist
            // 0 || 7 = Sonntag
            // 1 = Montag
            // 2 = Dienstag
            // 3 = Mittwoch
            // 4 = Donnerstag
            // 5 = Freitag
            // 6 = Samstag


            return std::make_pair(86'400 * 7, 7);
        }
        if (this->name == "year") {
            const auto currentYear = LeapYearUtils::getCurrentYear();
            const auto currentYearInSeconds = LeapYearUtils::seconds_since_1970();
            std::cout << "currentYearInSeconds: " << currentYearInSeconds << std::endl;

            const auto daysInCurrentYear = LeapYearUtils::getDaysInYear(currentYear);
            return std::make_pair(currentYearInSeconds, daysInCurrentYear);
        }

        std::cout << "CronPart::getPartRangeSize: unknown part name: " << this->name << std::endl;
        return std::make_pair(1, 0);
    }

    auto stepValue(const std::string &basicString) -> void {
        // */2
        // basicString is periodic
        auto components = StringUtils::split_by(basicString, '/');
        const auto step = std::stoi(components.at(1));

        for (int i = 0; i < this->range; i += step) {
            this->times.emplace_front(i * this->multiplier);
        }

    }

    auto rangeValue(const std::string &basicString) -> void {
        std::vector<std::string> range = StringUtils::split_by(basicString, '-');
        auto min = 0;
        auto max = 0;

        if(this->name == "weekday") {
            if(!StringUtils::is_number(range[0])) {
                min = LeapYearUtils::weekdayValue(range.at(0));
            }
            else{
                min = std::stoi(range.at(0));
            }

            if(!StringUtils::is_number(range[1])) {
                max = LeapYearUtils::weekdayValue(range.at(1));
            }
            else{
                max = std::stoi(range.at(1));
            }
        } else {
            min = std::stoi(range.at(0));
            max = std::stoi(range.at(1));
        }

        for (auto i = min; i <= max; ++i) {
            std::chrono::seconds currentTime(i);
            this->times.push_front(currentTime);
        }

    }

    auto listValue(const std::string &basicString) -> void {
        std::vector<std::string> list = StringUtils::split_by(basicString, ',');

        for (const auto &time: list) {
            if(this->name == "weekday") {
                if(!StringUtils::is_number(time)) {
                    this->times.emplace_front(LeapYearUtils::weekdayValue(time));
                }
                else{
                    this->times.emplace_front(std::stoi(time));
                }
            } else {
                this->times.emplace_front(std::stoi(time));
            }
        }
    }

    auto numberValue(const std::string &basicString) -> void {
        // 1
        // basicString is value

        int value = 0;
        if(this->name == "weekday" && !StringUtils::is_number(basicString)) {
            value = LeapYearUtils::weekdayValue(basicString);
        } else {
            value = std::stoi(basicString);
        }

        std::chrono::seconds currentTime(value * this->multiplier);
        this->times.emplace_front(currentTime);
    }

    auto periodValue(const std::string &basicString) -> void {
        // 1-5/2
        // basicString is periodic
        std::vector<std::string> periodic = StringUtils::split_by(basicString, '/');
        std::vector<std::string> range = StringUtils::split_by(periodic.at(0), '-');

        auto min = std::stoi(range.at(0));
        auto max = std::stoi(range.at(1));
        auto step = std::stoi(periodic.at(1));

        if(this->name == "weekday") {
            if(!StringUtils::is_number(range[0])) {
                min = LeapYearUtils::weekdayValue(range.at(0));
            }
            else{
                min = std::stoi(range.at(0));
            }

            if(!StringUtils::is_number(range[1])) {
                max = LeapYearUtils::weekdayValue(range.at(1));
            }
            else{
                max = std::stoi(range.at(1));
            }

            if(!StringUtils::is_number(periodic[1])) {
                step = LeapYearUtils::weekdayValue(periodic.at(1));
            }
            else{
                step = std::stoi(periodic.at(1));
            }
        }


        for (int i = min; i <= max; i += step) {
            std::chrono::seconds currentTime(i * this->multiplier);
            this->times.emplace_front(currentTime);
        }

    }




public:

    CronPart() = default;

    CronPart(std::string const &name, const std::string &rawValue)
            : name(name), rawValue(rawValue) {

        this->isNumber = StringUtils::is_number(rawValue);
        this->isWildcard = rawValue == "*";
        this->isRange = StringUtils::contains(rawValue, "-") && !StringUtils::contains(rawValue, "/");
        this->isList = StringUtils::contains(rawValue, ",");
        this->isPeriodic = StringUtils::contains(rawValue, "/") && !StringUtils::contains(rawValue, "*");

        const auto [multiplier, range] = getPartRangeSize();
        this->multiplier = multiplier;
        this->range = range;

        processValue(rawValue);

    }

    ~CronPart() = default;


    auto getName() const -> std::string {
        return name;
    }

    // -> std::deque<std::chrono::seconds>

    auto getTimes() const -> std::forward_list<std::chrono::seconds> {
        return times;
    }


};
