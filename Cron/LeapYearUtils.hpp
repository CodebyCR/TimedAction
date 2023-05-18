//
// Created by Christoph Rohde on 27.12.22.
//

#pragma once

#include <iostream>
#include <chrono>

using u_long = unsigned long;


namespace LeapYearUtils {

    auto getCurrentYear() {
        time_t t = time(nullptr);
        tm* timePtr = localtime(&t);
        return timePtr->tm_year;
    }

    auto getCurrentMonth() -> int {
        // Get the current time
        time_t currentTime = time(nullptr);

        // Convert the current time to a tm structure
        tm const* timeStruct = localtime(&currentTime);

        // Return the month field of the tm structure as an integer (0-based)
        return (timeStruct->tm_mon) + 1;
    }


    static auto isLeapYear(int year) -> bool {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    static auto getDaysInCurrentMonth() {
        auto year = getCurrentYear();
        auto month = getCurrentMonth();

        if(month == 2) {
            return isLeapYear(year) ? 29 : 28;
        }
        else if(month == 4 || month == 6 || month == 9 || month == 11) {
            return 30;
        }
        else {
            return 31;
        }
    }

    static auto getDaysInYear(int year) -> int {
        return isLeapYear(year) ? 366 : 365;
    }


    static auto getDaysInMonth(std::string_view month) -> int {
        if(month == "February") {
            return 29;
        }
        else if(month == "April" || month == "June" || month == "September" || month == "November") {
            return 30;
        }
        else {
            return 31;
        }
    }


    [[nodiscard]] auto weekdayValue(std::string_view const& dayName) -> uint8_t {
        // dayNameOfToday

        // weekday alias
        //        if (dayName == "SUN") {
        //            return 0;
        //        }
        if(dayName == "MON") {
            return 1;
        }
        if(dayName == "TUE") {
            return 2;
        }
        if(dayName == "WED") {
            return 3;
        }
        if(dayName == "THU") {
            return 4;
        }
        if(dayName == "FRI") {
            return 5;
        }
        if(dayName == "SAT") {
            return 6;
        }

        return 0;
    }


    auto getCurrentYearInSeconds() -> u_long {
        time_t t = time(nullptr);
        tm* timePtr = localtime(&t);
        time_t yearStart = mktime(timePtr);
        timePtr->tm_year += 1;
        timePtr->tm_mon = 0;
        timePtr->tm_mday = 1;
        time_t nextYearStart = mktime(timePtr);
        return nextYearStart - yearStart;
    }

    long getCurrentMonthInSeconds() {
        time_t t = time(nullptr);
        tm* timePtr = localtime(&t);
        time_t monthStart = mktime(timePtr);
        timePtr->tm_mon += 1;
        timePtr->tm_mday = 1;
        time_t nextMonthStart = mktime(timePtr);
        return nextMonthStart - monthStart;
    }

    /**
     * @return The current year in seconds since 1970
     */
    auto seconds_since_1970() -> int {
        std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(current_time);
        tm const* time_info = localtime(&time);
        int year = getCurrentYear();

        int days_since_1970 = (year - 1970) * 365;
        days_since_1970 += (year - 1968) / 4;
        days_since_1970 -= (year - 1900) / 100;
        days_since_1970 += (year - 1600) / 400;

        int seconds_since_1970 = days_since_1970 * 24 * 60 * 60;
        seconds_since_1970 += time_info->tm_yday * 24 * 60 * 60;
        seconds_since_1970 += time_info->tm_hour * 60 * 60;
        seconds_since_1970 += time_info->tm_min * 60;
        seconds_since_1970 += time_info->tm_sec;

        return seconds_since_1970;
    }

}    // namespace LeapYearUtils