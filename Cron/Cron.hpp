//
// Created by Christoph Rohde on 08.12.22.
//

#pragma once
#include <iostream>


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
class Cron{

private:
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayOfMonth;
    uint8_t month;
    std::string dayOfWeek; // array of days
    // 0-6 or sun-sat or sunday-saturday or mon-fri
    std::string year; // array of years

//    std::ostream& operator<<(std::ostream& os, const Cron& cron) {
//        os << "Cron: " << cron.second << " " << cron.minute << " " << cron.hour << " " << cron.dayOfMonth << " " << cron.month << " " << cron.dayOfWeek << " " << cron.year;
//        return os;
//    }

public:

//    // overload << operator to_string
//    friend std::ostream& operator << (std::ostream& os, const Cron& cron);
//
//    // overload >> operator from_string
//    friend std::istream& operator>>(std::istream& is, Cron& cron);

    auto operator <=> (const Cron& other) const = default;


    auto seconds(const uint8_t seconds) -> Cron& {
        this->second = seconds;
        return *this;
    }

    auto minutes(const uint8_t minutes) -> Cron& {
        this->minute = minutes;
        return *this;
    }

    auto hours(const uint8_t hours) -> Cron& {
        this->hour = hours;
        return *this;
    }

    auto daysOfMonth(const uint8_t daysOfMonth) -> Cron& {
        this->dayOfMonth = daysOfMonth;
        return *this;
    }

    auto months(const uint8_t months) -> Cron& {
        this->month = months;
        return *this;
    }

    auto daysOfWeek(const std::string_view& daysOfWeek) -> Cron& {
        this->dayOfWeek = daysOfWeek;
        return *this;
    }

    auto years(const std::string_view& years) -> Cron& {
        this->year = years;
        return *this;
    }

    [[nodiscard]] auto getSeconds() const -> uint8_t {
        return this->second;
    }

    [[nodiscard]] auto getMinutes() const -> uint8_t {
        return this->minute;
    }

    [[nodiscard]] auto getHours() const -> uint8_t {
        return this->hour;
    }

    [[nodiscard]] auto getDaysOfMonth() const -> uint8_t {
        return this->dayOfMonth;
    }

    [[nodiscard]] auto getMonths() const -> uint8_t {
        return this->month;
    }

    [[nodiscard]] auto getDaysOfWeek() const -> std::string {
        return this->dayOfWeek;
    }

    [[nodiscard]] auto getYears() const -> std::string {
        return this->year;
    }

    [[nodiscard]] auto toString() const -> std::string {
        return std::to_string(this->second) + " " + std::to_string(this->minute) + " " + std::to_string(this->hour) + " " + std::to_string(this->dayOfMonth) + " " + std::to_string(this->month) + " " + this->dayOfWeek + " " + this->year;
    }

};

