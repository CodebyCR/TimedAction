//
// Created by Christoph Rohde on 08.12.22.
//

#pragma once

#include <iostream>
#include <sstream>
#include <ctime>

#include "CronCapsule.hpp"
#include "CronPart.hpp"
#include "WeekdayPart.hpp"
#include "CronRegex.hpp"
#include "YearPart.hpp"
#include "Sort.hpp"



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
    WeekdayPart daysOfWeek; // 0-6 || MON || TUE || WED-THU || FRI || SAT || SUN || * || */2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String
    YearPart years;

    // 0-6 or sun-sat or sunday-saturday or mon-fri

//    std::ostream& operator<<(std::ostream& os, const Cron& cron) {
//        os << "Cron: " << cron.second << " " << cron.minute << " " << cron.hour << " " << cron.dayOfMonth << " " << cron.month << " " << cron.dayOfWeek << " " << cron.year;
//        return os;
//    }

    auto processCronParts(std::vector<std::string> &cronParts) -> void {
        if (!CronRegex::isValidCron(cronParts)) {
            throw std::invalid_argument("Invalid cron string");
        }

        if (cronParts.size() != 7) {
            throw std::invalid_argument("Cron string has to have 6 parts");
        }

        this->seconds = CronPart("second", cronParts[0]);
        this->minutes = CronPart("minute", cronParts[1]);
        this->hours = CronPart("hour", cronParts[2]);
        this->daysOfMonth = CronPart("day", cronParts[3]);
        this->months = CronPart("month", cronParts[4]);
        this->daysOfWeek = WeekdayPart(cronParts[5]);
        this->years = YearPart(cronParts[6]);
    }

    /**
     * Create time points from the cron parts of the cron object.
     * @param cronObject
     * @return a vector of unfiltered time points
     */
    [[nodiscard]]
    auto cartesian_product() const {
        std::vector<std::tm> resultTime; // to list from time points

        for (auto const &year: years.getTimes()) {
            auto yearVal = std::chrono::duration_cast<std::chrono::years>(year);

            for (auto const &month: months.getTimes()) {
                auto monthVal = std::chrono::duration_cast<std::chrono::months>(month);

                for (auto const &dayOfMonth: daysOfMonth.getTimes()) {
                    auto dayVal = std::chrono::duration_cast<std::chrono::days>(dayOfMonth);

                    for (auto const &hour: hours.getTimes()) {
                        auto hourVal = std::chrono::duration_cast<std::chrono::hours>(hour);

                        for (auto const &minute: minutes.getTimes()) {
                            auto minuteVal = std::chrono::duration_cast<std::chrono::minutes>(minute);

                            for (auto const &second: seconds.getTimes()) {

                                auto timeStruct = std::tm();
                                timeStruct.tm_sec = second.count();
                                timeStruct.tm_min = minuteVal.count();
                                timeStruct.tm_hour = hourVal.count();
                                timeStruct.tm_mday = dayVal.count();
                                timeStruct.tm_mon = monthVal.count();
                                timeStruct.tm_year = yearVal.count();

                                resultTime.push_back(timeStruct);
                            }
                        }
                    }
                }
            }
        }

        return resultTime;
    }

    /**
     * Filtered unreachable time points out of the cartesian product.
     * @param cartesianProduct
     * @return A filtered vector of time points.
     */
    [[nodiscard]]
    auto filterOfReachedTimes(const std::vector<std::tm> &cartesianProduct) const {
        std::vector<std::tm> result;

        auto now = std::chrono::system_clock::now();
        auto nowSeconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

        for (auto time: cartesianProduct) {
            time.tm_year -= 1900;
            auto currentTime = std::mktime(&time);
            auto currentTime_ = std::chrono::system_clock::from_time_t(currentTime);
            const auto timeDifferance = currentTime - nowSeconds.count();

            if (timeDifferance > 0) {
                result.push_back(time);
            }
        }

        return result;
    }

    /**
     * Sort the time points by valid Weekdays.
     * @param times
     */
     [[nodiscard]]
     auto filteredOfWeekdayPart(const std::vector<std::tm> &times,
                                      const std::vector<int> &weekdays) const {

        std::vector<std::tm> result;

        for (auto const& time: times) {
            for (auto const& weekday : weekdays) {

                if (time.tm_wday == weekday) {
                    result.push_back(time);
                }
            }
        }

        return result;
    }

    /**
     * Filter the cartesian product of the cron object.
     * @param cartesianProduct
     * @return A filtered vector of time points that are sorted by the next reached time.
     */
     [[nodiscard]]
     auto get_time_points() const -> std::vector<std::tm> {
        auto cartesianProduct = cartesian_product();
        auto filteredOfReachedTimes = filterOfReachedTimes(cartesianProduct);
        auto totalTimes = filteredOfWeekdayPart(filteredOfReachedTimes, daysOfWeek.getContainedWeekdays());

        Sort::by_next_reached_time(totalTimes);

        return totalTimes;
    }

public:

    explicit Cron(CronCapsule capsule) {
        
        const auto& [second, minute, hour, dayOfMonth, month, weekday, year] = capsule;
        
        std::vector<std::string> cronParts = {
                second, minute, hour, dayOfMonth, month, weekday, year};

        processCronParts(cronParts);
    }

    explicit Cron(std::string const& cronString) {
        auto cronParts = StringUtils::split_by(cronString, ' ');
        processCronParts(cronParts);
    }

     friend std::ostream& operator<<(std::ostream &os, const Cron &cron)   {
         auto first_execution = cron.get_time_points().at(0);
         std::time_t time = std::mktime(&first_execution);
         auto pretty_time = std::ctime(&time);
        os << pretty_time;
        return os;
    }

    friend std::istream& operator>>(std::istream &is, Cron &cron) {
        std::string cronString;
        is >> cronString;
        cron = Cron(cronString);
        return is;
    }

    friend bool operator < (const Cron &lhs, const Cron &rhs) {
        auto lhs_first_execution = lhs.get_time_points().at(0);
        auto rhs_first_execution = rhs.get_time_points().at(0);
        return std::mktime(&lhs_first_execution) < std::mktime(&rhs_first_execution);
    }

    friend bool operator > (const Cron &lhs, const Cron &rhs) {
        auto lhs_first_execution = lhs.get_time_points().at(0);
        auto rhs_first_execution = rhs.get_time_points().at(0);
        return std::mktime(&lhs_first_execution) > std::mktime(&rhs_first_execution);
    }

    friend Cron operator "" _cron(const char* str, std::size_t size) {
         auto cron_expression = Cron(std::string(str, size));
        return Cron(cron_expression);
    }

    [[nodiscard]]
    auto getSecondTimes() const {
        return this->seconds.getTimes();
    }

    [[nodiscard]]
    auto getMinuteTimes() const {
        return this->minutes.getTimes();
    }

    [[nodiscard]]
    auto getHourTimes() const {
        return this->hours.getTimes();
    }

    [[nodiscard]]
    auto getDayOfMonthTimes() const {
        return this->daysOfMonth.getTimes();
    }

    [[nodiscard]]
    auto getMonthTimes() const {
        return this->months.getTimes();
    }

    [[nodiscard]]
    auto getWeekDayTimes() const {
        return this->daysOfWeek.getContainedWeekdays();
    }

    [[nodiscard]]
    auto getYearTimes() const {
        return this->years.getTimes();
    }


    [[nodiscard]]
    auto get_execution_times() const -> std::vector<std::tm>
    {
        return get_time_points();
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
    auto to_cron(const std::string &cronString) -> Cron {
        return Cron(cronString);
    }

//    auto operator "" _cron(const char* cron_expression)
//    {
//        return Cron(cron_expression);
//    }
}

namespace CronExpression{

//    std::shared_ptr<Cron> cron;
    static auto everyMinute() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("*/1 * * * * * *");
    }

    static auto everyFiveMinutes() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("*/5 * * * * * *");
    }

    static auto everyTenMinutes() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("*/10 * * * * * *");
    }

    static auto everyFifteenMinutes() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("*/15 * * * * * *");
    }

    static auto everyThirtyMinutes() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("*/30 * * * * * *");
    }

    static auto everyHour() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("0 */1 * * * * *");
    }

    static auto everyTwoHours() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("0 */2 * * * * *");
    }

    static auto everyDay() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("0 0 12 * * * *");
    }

    static auto everyTwoDays() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("0 0 12 */2 * * *");
    }

    static auto everyWeek() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("0 0 12 * * Mon *");
    }

    static auto everyMonth() -> std::shared_ptr<Cron>
    {
        return std::make_shared<Cron>("0 0 12 1 * * *");
    }

}
