//
// Created by Christoph Rohde on 08.12.22.
//

#pragma once

#include <iostream>
#include <sstream>


#include <CronPart.hpp>
#include <CronRegex.hpp>


/**
 * A class to create a cron format in a more readable way.
 * Using a builder pattern.
 *
 * Example:
 * auto cron = Cron();
 * cron.seconds(1)
 *    .minutes(1)
 *    .hours(1)
 *    .daysOfMonth(1)
 *    .months(1)
 *    .daysOfWeek("mon-fri")
 *    .years("2021");
 */
class Cron {

private:
    CronPart seconds;
    CronPart minutes; // 0-59 || * || */2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String
    CronPart hours; // 0-23 || * || */2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String
    CronPart daysOfMonth; // 1-31 || * || */2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String
    CronPart months; // 1-12 || * || */2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String

    CronPart daysOfWeek; // 0-6 || MON || TUE || WED-THU || FRI || SAT || SUN || * || */2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String
    CronPart years;

    // 0-6 or sun-sat or sunday-saturday or mon-fri


//    std::ostream& operator<<(std::ostream& os, const Cron& cron) {
//        os << "Cron: " << cron.second << " " << cron.minute << " " << cron.hour << " " << cron.dayOfMonth << " " << cron.month << " " << cron.dayOfWeek << " " << cron.year;
//        return os;
//    }

public:

    explicit Cron(std::string cronString) {
        auto cronParts = StringUtils::split_by(cronString, ' ');

        if (!CronRegex::isValidCron(cronParts)){
            throw std::invalid_argument("Invalid cron string");
        }

        if(cronParts.size() != 7){
            throw std::invalid_argument("Cron string has to have 6 parts");
        }

        this->seconds = CronPart("second", cronParts[0]);
        this->minutes = CronPart("minute", cronParts[1]);
        this->hours = CronPart("hour", cronParts[2]);
        this->daysOfMonth = CronPart("day", cronParts[3]);

        this->months = CronPart("month", cronParts[4]);
        this->daysOfWeek = CronPart("weekday", cronParts[5]);
        this->years = CronPart("year", cronParts[6]);
    }

//    auto operator<<(std::ostream &os) const -> std::ostream & {
//        os << this->seconds
//           << " " << this->minute.value_or(0)
//           << " " << this->hour.value_or(0)
//           << " " << this->dayOfMonth.value_or(1)
//           << " " << this->month.value_or(0)
//           << " " << this->dayOfWeek
//           << " " << this->year;
//        return os;
//    }


//    // overload >> operator from_string
//    friend std::istream& operator>>(std::istream& is, Cron& cron);

    auto operator<=>(const Cron &other) const = default;


//    auto seconds(const uint8_t seconds) -> Cron & {
//        this->second = seconds;
//        return *this;
//    }
//
//    auto minutes(const uint8_t minutes) -> Cron & {
//        this->minute = minutes;
//        return *this;
//    }
//
//    auto hours(const uint8_t hours) -> Cron & {
//        this->hour = hours;
//        return *this;
//    }
//
//    auto daysOfMonth(const uint8_t daysOfMonth) -> Cron & {
//        this->dayOfMonth = daysOfMonth;
//        return *this;
//    }
//
//    auto months(const uint8_t months) -> Cron & {
//        this->month = months;
//        return *this;
//    }
//
//    auto daysOfWeek(const std::string_view &daysOfWeek) -> Cron & {
//        this->dayOfWeek = daysOfWeek;
//        return *this;
//    }
//
//    auto years(const std::string_view &years) -> Cron & {
//        this->year = years;
//        return *this;
//    }
//
//    [[nodiscard]]
//    auto getSeconds() const -> uint8_t {
//        return this->second;
//    }
//
//    [[nodiscard]]
//    auto getMinutes() const -> uint8_t {
//        return this->minute;
//    }
//
//    [[nodiscard]]
//    auto getHours() const -> uint8_t {
//        return this->hour;
//    }
//
//    [[nodiscard]]
//    auto getDaysOfMonth() const {
//        return this->daysOfMonth;
//    }
//
//    [[nodiscard]]
//    auto getMonths() const {
//        return this->months;
//    }
//
//    [[nodiscard]]
//    auto getDaysOfWeek() const {
//        return this->daysOfWeek;
//    }
//
//    [[nodiscard]]
//    auto getYears() const {
//        return this->years;
//    }
//
//    [[nodiscard]]
//    auto toString() const {
//        std::stringstream ss;
//        ss << this->second
//           << " " << this->minute
//           << " " << this->hour
//           << " " << this->dayOfMonth
//           << " " << this->month
//           << " " << this->dayOfWeek
//           << " " << this->year;
//        return ss.str();
//    }

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
        return this->daysOfWeek.getTimes();
    }

    [[nodiscard]]
    auto getYearTimes() const {
        return this->years.getTimes();
    }

};

