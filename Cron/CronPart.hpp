//
// Created by Christoph Rohde on 24.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include "../StringUtils.hpp"
#include <forward_list>
#include "LeapYearUtils.hpp"


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

    u_long partMultiplier;
    uint8_t partRange;


    /** 1-31 || * || *\/2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String */
    void processValue(const std::string &basicString) {

        if (isNumber) {
            numberValue(basicString);
        }

        if (this->isWildcard) {
            // *
            if (this->name == "year") {
                auto current_year = LeapYearUtils::getCurrentYear();
                // Current year to str
                std::cout << "Current year: " << current_year << std::endl;
                this->times.emplace_front(current_year);
            }
//            else if (this->name == "hour") {
//                // TODO: Stunden haben eine stunde versatz
//                for (int index = 0; index < this->partRange; index++) {
//                    this->times.emplace_front(index  * this->partMultiplier);
//                }
//
//            }
            else
            {
                /// Range works
                for (int index = 0; index < this->partRange; index++) {
                    this->times.emplace_front((index + 1) * this->partMultiplier);
                }
            }
        }

        if (StringUtils::contains(basicString, "*") && basicString.length() > 1) {
            // */2
            stepValue(basicString);
        }

        if (this->isRange) {
            // 1-5
            rangeValue(basicString);
        }

        if (this->isList) {
            // 1,2,3
            listValue(basicString);
        }

        if (this->isPeriodic) {
            // 1-5/2
            periodValue(basicString);
        }

    }

    /**
     * @return partMultiplier (value of section in seconds) & partRange (max value of section)
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


    /** processing of step values */
    auto stepValue(const std::string &basicString) -> void {
        // */2
        auto components = StringUtils::split_by(basicString, '/');
        const auto step = std::stoi(components.at(1));

        for (int i = 0; i < this->partRange; i += step) {
            this->times.emplace_front(i * this->partMultiplier);
        }

    }

    /** processing of list values */
    auto rangeValue(const std::string &basicString) -> void {
        std::vector<std::string> range = StringUtils::split_by(basicString, '-');
        auto min = 0;
        auto max = 0;

        if (this->name == "weekday") {
            if (!StringUtils::is_number(range[0])) {
                min = LeapYearUtils::weekdayValue(range.at(0));
            } else {
                min = std::stoi(range.at(0));
            }

            if (!StringUtils::is_number(range[1])) {
                max = LeapYearUtils::weekdayValue(range.at(1));
            } else {
                max = std::stoi(range.at(1));
            }
        } else if (this->name == "year"){
            min= std::stoi(range.at(0));
            max = std::stoi(range.at(1));

            for (auto index = min; index <= max; ++index) {
                const auto year_in_seconds = std::chrono::years(index);
                this->times.push_front(year_in_seconds);
            }
            return;
        } else {
            min = std::stoi(range.at(0));
            max = std::stoi(range.at(1));
        }

        for (auto i = min; i <= max; ++i) {
            std::chrono::seconds currentTime(i );
            this->times.push_front(currentTime);
        }

    }

    /** processing of list values */
    auto listValue(const std::string &basicString) -> void {
        std::vector<std::string> list = StringUtils::split_by(basicString, ',');

        for (const auto &time: list) {
            if (this->name == "weekday") {
                if (!StringUtils::is_number(time)) {
                    this->times.emplace_front(LeapYearUtils::weekdayValue(time));
                } else {
                    this->times.emplace_front(std::stoi(time));
                }
            } else {
                this->times.emplace_front(std::stoi(time));
            }
        }
    }

    /** processing of number values */
    auto numberValue(const std::string &basicString) -> void {

        if(this->name == "year") {
            const auto year_count = std::stoi(basicString);
            const auto year_in_seconds = std::chrono::years(year_count);
            this->times.emplace_front(year_in_seconds);
            return;
        }

        // 1
        int value = 0;
        if (this->name == "weekday" && !StringUtils::is_number(basicString)) {
            value = LeapYearUtils::weekdayValue(basicString);
        } else {
            value = std::stoi(basicString);
        }

        std::chrono::seconds currentTime(value * this->partMultiplier);
        this->times.emplace_front(currentTime);
    }

    /** processing of period values */
    auto periodValue(const std::string &basicString) -> void {
        // 1-5/2
        // basicString is periodic
        std::vector<std::string> periodic = StringUtils::split_by(basicString, '/');
        std::vector<std::string> range = StringUtils::split_by(periodic.at(0), '-');

        auto min = std::stoi(range.at(0));
        auto max = std::stoi(range.at(1));
        auto step = std::stoi(periodic.at(1));

        if (this->name == "weekday") {
            if (!StringUtils::is_number(range[0])) {
                min = LeapYearUtils::weekdayValue(range.at(0));
            } else {
                min = std::stoi(range.at(0));
            }

            if (!StringUtils::is_number(range[1])) {
                max = LeapYearUtils::weekdayValue(range.at(1));
            } else {
                max = std::stoi(range.at(1));
            }

            if (!StringUtils::is_number(periodic[1])) {
                step = LeapYearUtils::weekdayValue(periodic.at(1));
            } else {
                step = std::stoi(periodic.at(1));
            }
        }

        for (int i = min; i <= max; i += step) {
            std::chrono::seconds currentTime(i * this->partMultiplier);
            this->times.emplace_front(currentTime);
        }

    }


public:

    CronPart() = default;

    CronPart(std::string name, const std::string &rawValue):
            name(std::move(name)), rawValue(rawValue) {

        this->isNumber = StringUtils::is_number(rawValue);
        this->isWildcard = rawValue == "*";
        this->isRange = StringUtils::contains(rawValue, "-") && !StringUtils::contains(rawValue, "/");
        this->isList = StringUtils::contains(rawValue, ",");
        this->isPeriodic = StringUtils::contains(rawValue, "/") && !StringUtils::contains(rawValue, "*");

        const auto [multiplier, range] = getPartRangeSize();
        this->partMultiplier = multiplier;
        this->partRange = range;

        processValue(rawValue);
    }

    ~CronPart() = default;

    [[nodiscard]]
    auto getName() const -> std::string {
        return name;
    }

    [[nodiscard]]
    auto getTimes() const -> std::forward_list<std::chrono::seconds> {
        return times;
    }

};