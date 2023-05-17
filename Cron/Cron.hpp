//
// Created by Christoph Rohde on 08.12.22.
//

#pragma once

#include <ctime>
#include <iostream>
#include <sstream>

#include "CronCapsule.hpp"
#include "CronPart.hpp"
#include "CronRegex.hpp"
#include "Sort.hpp"
#include "WeekdayPart.hpp"
#include "YearPart.hpp"
#include "ExecutionTimeGenerator.hpp"


/**
 * A class to create a cron format in a more readable way.
 * Using a builder pattern.
 *
 * Example:
 * auto cron =  Cron({.second = "0",
 *                    .minute = "30",
 *                    .hour = "*\/2",
 *                    .dayOfMonth = "15",
 *                    .month = "*\/2",
 *                    .weekday = "*",
 *                    .year = "2023-2024"});
 */
class Cron {
private:
    CronPart seconds;
    CronPart minutes;
    CronPart hours;
    CronPart daysOfMonth;
    CronPart months;
    WeekdayPart daysOfWeek;
    YearPart years;

    ExecutionTimeGenerator generator;
    std::vector<std::tm> execution_times;

    auto processCronParts(std::vector<std::string>& cronParts) -> void {
        if(!CronRegex::isValidCron(cronParts)) {
            throw std::invalid_argument("Invalid cron string");
        }

        if(cronParts.size() != 7) {
            throw std::invalid_argument("Cron string has to have 6 parts");
        }

        this->seconds       = CronPart("second",    cronParts[0]);
        this->minutes       = CronPart("minute",    cronParts[1]);
        this->hours         = CronPart("hour",      cronParts[2]);
        this->daysOfMonth   = CronPart("day",       cronParts[3]);
        this->months        = CronPart("month",     cronParts[4]);
        this->daysOfWeek    = WeekdayPart(          cronParts[5]);
        this->years         = YearPart(             cronParts[6]);
    }

    /// Constructor processing <br/>
    /// For outsourcing the constructor logic of the different constructors
    auto constructor_processing(std::vector<std::string>& cronParts) -> void {
        //std::cout << "-1-" << std::endl;
        if(!CronRegex::isValidCron(cronParts)) {
            throw std::invalid_argument("Invalid cron string");
        }
        //std::cout << "-2-" << std::endl;
        if(cronParts.size() != 7) {
            throw std::invalid_argument("Cron string has to have 6 parts");
        }
        //std::cout << "-3-" << std::endl;
        processCronParts(cronParts);
        //std::cout << "-4-" << std::endl;

        generator = ExecutionTimeGenerator::generate_from(
                seconds.getTimes(),
                minutes.getTimes(),
                hours.getTimes(),
                daysOfMonth.getTimes(),
                months.getTimes(),
                years.getTimes(),

                daysOfWeek.getContainedWeekdays());

//        generator();
        execution_times = generator();
//        this->resume_execution_times();
        std::cout << "-5-" << execution_times.size() << std::endl;
        Sort::by_next_reached_time(execution_times);

        std::cout << "Constructor processing" << std::endl;
    }


public:
    explicit Cron(const CronCapsule& capsule) {
        const auto& [second, minute, hour, dayOfMonth, month, weekday, year] = capsule;

        std::vector<std::string> cronParts = {
                second, minute, hour, dayOfMonth, month, weekday, year};

        constructor_processing(cronParts);

        std::cout << "Cron Capsule " << std::endl;
    }

    explicit Cron(std::string const& cronString) {

        std::cout << "Cron String: " << cronString << std::endl;
        if(cronString.empty()) {
            throw std::invalid_argument("Cron string can't be empty.");
        }

        if(!cronString.contains(' ')) {
            throw std::invalid_argument("Wrong cron string format exception.");
        }

        auto cronParts = StringUtils::split_by(cronString, ' ');

        constructor_processing(cronParts);
     }



    friend std::ostream& operator<<(std::ostream& os, Cron& cron) {
        auto first_execution = cron.get_execution_times().at(0);
        std::time_t time = std::mktime(&first_execution);
        auto pretty_time = std::ctime(&time);
        os << pretty_time;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Cron& cron) {
        std::string cronString;
        is >> cronString;
        cron = Cron(cronString);
        return is;
    }

    friend bool operator<( Cron& lhs, Cron& rhs) {
        auto lhs_first_execution = lhs.get_execution_times().at(0);
        auto rhs_first_execution = rhs.get_execution_times().at(0);
        return std::mktime(&lhs_first_execution) < std::mktime(&rhs_first_execution);
    }

    friend bool operator>(Cron& lhs, Cron& rhs) {
        auto lhs_first_execution = lhs.get_execution_times().at(0);
        auto rhs_first_execution = rhs.get_execution_times().at(0);
        return std::mktime(&lhs_first_execution) > std::mktime(&rhs_first_execution);
    }

    friend Cron operator""_cron(const char* str, std::size_t size) {
        auto cron_expression = Cron(std::string(str, size));
        return cron_expression;
    }

    [[nodiscard]] auto getSecondTimes()  {
        return this->seconds.getTimes();
    }

    [[nodiscard]] auto getMinuteTimes()  {
        return this->minutes.getTimes();
    }

    [[nodiscard]] auto getHourTimes()  {
        return this->hours.getTimes();
    }

    [[nodiscard]] auto getDayOfMonthTimes()  {
        return this->daysOfMonth.getTimes();
    }

    [[nodiscard]] auto getMonthTimes()  {
        return this->months.getTimes();
    }

    [[nodiscard]] auto getWeekDayTimes() const {
        return this->daysOfWeek.getContainedWeekdays();
    }

    [[nodiscard]] auto getYearTimes() {
        return this->years.getTimes();
    }


    [[nodiscard]] auto get_execution_times() -> std::vector<std::tm> {
        return execution_times;
    }

    auto resume_execution_times() -> Cron {
        execution_times = generator();
        Sort::by_next_reached_time(execution_times);
        return *this;
    }


};

//namespace std {
//    template<>
//    struct hash<Cron> {
//        auto operator()(const Cron &cron) const -> size_t {
//            return hash<std::string>()(cron);
//        }
//    };
//}

namespace std {
    auto to_cron(const std::string& cronString) -> Cron {
        return Cron(cronString);
    }

    //    auto operator "" _cron(const char* cron_expression)
    //    {
    //        return Cron(cron_expression);
    //    }
}    // namespace std

namespace CronExpression {
    // ! TODO: dont works
    //    std::shared_ptr<Cron> cron;

    constexpr std::string_view every_minute = "*/1 * * * * * *";

    static auto everyFiveMinutes() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("*/5 * * * * * *");
    }

    static auto everyTenMinutes() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("*/10 * * * * * *");
    }

    static auto everyFifteenMinutes() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("*/15 * * * * * *");
    }

    static auto everyThirtyMinutes() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("*/30 * * * * * *");
    }

    static auto everyHour() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("0 */1 * * * * *");
    }

    static auto everyTwoHours() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("0 */2 * * * * *");
    }

    static auto everyDay() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("0 0 12 * * * *");
    }

    static auto everyTwoDays() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("0 0 12 */2 * * *");
    }

    static auto everyWeek() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("0 0 12 * * Mon *");
    }

    static auto everyMonth() -> std::shared_ptr<Cron> {
        return std::make_shared<Cron>("0 0 12 1 * * *");
    }

}    // namespace CronExpression
