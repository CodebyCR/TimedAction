//
// Created by Christoph Rohde on 08.12.22.
//

#pragma once

#include <iostream>
#include <sstream>

#include "CronCapsule.hpp"
#include "CronPart.hpp"
#include "WeekdayPart.hpp"
#include "CronRegex.hpp"


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
    CronPart years;

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
        this->daysOfWeek = WeekdayPart( cronParts[5]);
        this->years = CronPart("year", cronParts[6]);
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

//    auto operator<=>(const Cron &other) const = default;

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




