//
// Created by Christoph Rohde on 24.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <../StringUtils.hpp>
#include <forward_list>

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

        if ( isNumber){
            numberValue(basicString);
        }

        if(this->isWildcard){
            // *
            // basicString is wildcard
            for(int index = 0; index < this->range; index++){
                this->times.emplace_front(index * this->multiplier);
            }

        }
        if(StringUtils::contains(basicString, "*") && basicString.length() > 1){
            // */2
            // basicString is periodic
            stepValue(basicString);
        }


        if(this->isRange){
            // 1-5
            // basicString is range
            std::vector<std::string> range = StringUtils::split_by(basicString, '-');
            const auto min = std::stoi(range.at(0));
            const auto max = std::stoi(range.at(1));

            for (auto i = min; i < max; ++i) {
                std::chrono::seconds s(i);
                this->times.push_front(s);
            }
        }



        if(this->isList){
            // 1,2,3
            // basicString is list
            std::vector<std::string> list = StringUtils::split_by(basicString, ',');
            std::vector<int> listValues;

            for (const auto &time : list) {
                this->times.emplace_front(std::stoi(time));
            }

        }



        if(this->isPeriodic){
            // 1-5/2
            periodValue(basicString);
        }

    }

    /**
     * @return multiplier (value of section in seconds) & range (max value of section)
     */
    [[nodiscard]]
    auto getPartRangeSize() const -> std::pair<u_long , uint8_t> {

        if (this->name == "second") {
            return std::make_pair( 1, 60);
        }
        if (this->name == "minute") {
            return std::make_pair( 60, 60);
        }
        if (this->name == "hour") {
            return std::make_pair( 3600, 24);
        }
        if (this->name == "day") {
            return std::make_pair( 86400, 31);
        }
        if (this->name == "month") {
            // TODO: 31 days in month

            return std::make_pair( 86400 * 30 , 12);
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

            return std::make_pair( 86400 * 7, 7);
        }
        if (this->name == "year") {
            // TODO: make working with leap years
            return std::make_pair( 86400 * 30 * 12, 365);
        }

        std::cout << "CronPart::getPartRangeSize: unknown part name: " << this->name << std::endl;
        return std::make_pair( 1, 0);
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

    auto rangeValue(const std::string &basicString) -> void
    {
        std::vector<std::string> range = StringUtils::split_by(basicString, '-');
        const auto min = std::stoi(range.at(0));
        const auto max = std::stoi(range.at(1));

        for (auto i = min; i <= max; ++i) {
            std::chrono::seconds currentTime(i);
            this->times.push_front(currentTime);
        }

    }

    auto listValue(const std::string &basicString) -> void
    {
        std::vector<std::string> list = StringUtils::split_by(basicString, ',');

        for (const auto &item : list) {
            std::chrono::seconds currentTime(std::stoi(item) * this->multiplier);
            this->times.emplace_front(currentTime);
        }
    }

    auto numberValue(const std::string &basicString) -> void
    {
        // 1
        // basicString is value
        std::chrono::seconds currentTime(std::stoi(basicString) * this->multiplier);
        this->times.emplace_front(currentTime);
    }

    auto periodValue(const std::string &basicString) -> void
    {
        // 1-5/2
        // basicString is periodic
        std::vector<std::string> periodic = StringUtils::split_by(basicString, '/');
        std::vector<std::string> range = StringUtils::split_by(periodic.at(0), '-');

        const auto min = std::stoi(range.at(0));


        const auto max = std::stoi(range.at(1));
        const auto step = std::stoi(periodic.at(1));

        for (int i = min; i <= max; i += step) {
            std::chrono::seconds currentTime(i * this->multiplier);
            this->times.emplace_front(currentTime);
        }

    }


public:



    CronPart( std::string const& name, const std::string &rawValue)
        : name(name), rawValue(rawValue){

        this-> isNumber = StringUtils::is_number(rawValue);
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
