//
// Created by Christoph Rohde on 24.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include "../Utilities/StringUtils.hpp"
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
////            else if (this->name == "hour") {
////                // TODO: Stunden haben eine stunde versatz -> wahrscheinlich wegen der Winterzeit
////                for (int index = 0; index < this->partRange; index++) {
////                    this->times.emplace_front(index  * this->partMultiplier);
////                }
////
////            }
//            else
//            {
            /// Range works
            if(name == "month") {
                for (int index = 0; index < this->partRange; index++) {
                    this->times.emplace_front((index) * this->partMultiplier);
                }
            }
            else {
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
//            std::cout << "monthMultiplier: " << monthMultiplier << std::endl;

            // seconds for the specific month in the current year, multiplied by
            return std::make_pair(86'400 * monthMultiplier, 12);
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
        auto min = std::stoi(range.at(0));
        auto max = std::stoi(range.at(1));

        for (auto i = min; i <= max; ++i) {
            std::chrono::seconds currentTime(i );
            this->times.push_front(currentTime);
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
        // 1
        int value = std::stoi(basicString);

        if(this->name == "month") {
            value -= 1;
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
        this->isRange = rawValue.contains("-") && !rawValue.contains('/');
        this->isList = rawValue.contains(",");
        this->isPeriodic = rawValue.contains("/") && !rawValue.contains("*");

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