//
// Created by Christoph Rohde on 29.01.23.
//

#pragma once

#include <iostream>
#include <string>
#include "../StringUtils.hpp"
#include <forward_list>
#include "LeapYearUtils.hpp"

class YearPart {

private:
    std::string name = "year";
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
            auto current_year = LeapYearUtils::getCurrentYear();
            // Current year to str
            std::cout << "Current year: " << current_year << std::endl;
            this->times.emplace_front(current_year);
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
    static auto getPartRangeSize() -> std::pair<u_long, uint8_t> {
        const auto currentYear = LeapYearUtils::getCurrentYear();
        const auto currentYearInSeconds = LeapYearUtils::seconds_since_1970();

        const auto daysInCurrentYear = LeapYearUtils::getDaysInYear(currentYear);
        return std::make_pair(currentYearInSeconds, daysInCurrentYear);
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
        auto min= std::stoi(range.at(0));
        auto max = std::stoi(range.at(1));

        for (auto index = min; index <= max; ++index) {
            const auto year_in_seconds = std::chrono::years(index);
            this->times.emplace_front(year_in_seconds);
        }

    }

    /** processing of list values */
    auto listValue(const std::string &basicString) -> void {
        std::vector<std::string> list = StringUtils::split_by(basicString, ',');

        for (const auto &time: list) {
                this->times.emplace_front(std::stoi(time));
        }
    }

    /** processing of number values */
    auto numberValue(const std::string &basicString) -> void {
        const auto year_count = std::stoi(basicString);
        const auto year_in_seconds = std::chrono::years(year_count);
        this->times.emplace_front(year_in_seconds);
    }

    /** processing of period values */
    auto periodValue(const std::string &basicString) -> void {
        // 1-5/2
        // basicString is periodic
        std::vector<std::string> periodic = StringUtils::split_by(basicString, '/');
        std::vector<std::string> range = StringUtils::split_by(periodic.at(0), '-');

        const auto min = std::stoi(range.at(0));
        const auto max = std::stoi(range.at(1));
        const auto step = std::stoi(periodic.at(1));

        for (int i = min; i <= max; i += step) {
            std::chrono::seconds currentTime(i * this->partMultiplier);
            this->times.emplace_front(currentTime);
        }
    }


public:

    YearPart() = default;

    YearPart(const std::string &rawValue):
            rawValue(rawValue) {

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

    ~YearPart() = default;


    [[nodiscard]]
    auto getName() const -> std::string {
        return name;
    }

    [[nodiscard]]
    auto getTimes() const -> std::forward_list<std::chrono::seconds> {
        return times;
    }

};
