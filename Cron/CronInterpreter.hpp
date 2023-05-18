//
// Created by Christoph Rohde on 28.12.22.
//

#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "Cron.hpp"


namespace CronInterpreter {

    auto get_year_dif_in_sec() -> long long {    //Current year as seconds
        auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch());
        auto yearVal = std::chrono::duration_cast<std::chrono::years>(currentTime);
        auto currentYear = yearVal.count();

        // 53 as seconds
        auto weekDaySeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::years(53));
        auto count = weekDaySeconds.count();
        return count;
    }

    auto get_month_from_index(const int index) { // put in month part
        auto months = std::vector {"January",
                                   "February",
                                   "March",
                                   "April",
                                   "May",
                                   "June",
                                   "July",
                                   "August",
                                   "September",
                                   "October",
                                   "November",
                                   "December"};

        return months[index];
    }


    /// Prints the time points in a table
    static auto get_info(std::vector<std::tm> const& timePoints) -> std::string {
        std::cout << "Valid entries: " << timePoints.size() << std::endl;

        auto ss = std::stringstream();
        ss << "\n\nHour Minute Second DayOfMonth     Month Year" << std::endl;
        ss << "------------------------------------------------" << std::endl;

        for(auto const& timeStruct: timePoints) {
            ss << std::setfill(' ') << std::setw(4) << timeStruct.tm_hour << " "
               << std::setfill(' ') << std::setw(6) << timeStruct.tm_min << " "
               << std::setfill(' ') << std::setw(6) << timeStruct.tm_sec << " "
               << std::setfill(' ') << std::setw(10) << timeStruct.tm_mday << " "
               << std::setfill(' ') << std::setw(9) << get_month_from_index(timeStruct.tm_mon) << " "
               << std::setfill(' ') << std::setw(4) << timeStruct.tm_year + 1900
               << std::endl;
        }

        return ss.str();
    }

    static auto pretty_print(std::vector<std::tm>& timePoints) {
        for(auto& timeStruct: timePoints) {
            std::time_t time = std::mktime(&timeStruct);
            std::cout << std::ctime(&time);
        }
    }


    /// <h3>This function is for debugging purposes.</h3>
    /// <br/>
    /// It shows all the extracted values from the cron string,
    /// which are used to calculate the time points
    static auto each_print(Cron cron) {
        std::cout << "Second:\n\t" << std::flush;
        for(auto const& entry: cron.getSecondTimes()) {
            std::cout << entry.count() << " ";
        }
        std::cout << std::endl;

        std::cout << "Minute:\n\t" << std::flush;
        for(auto const& entry: cron.getMinuteTimes()) {
            auto minute = duration_cast<std::chrono::minutes>(entry);
            std::cout << minute.count() << " ";
        }
        std::cout << std::endl;

        std::cout << "Hour:\n\t" << std::flush;
        for(auto const& entry: cron.getHourTimes()) {
            auto hour = duration_cast<std::chrono::hours>(entry);
            std::cout << hour.count() << " ";
        }
        std::cout << std::endl;

        std::cout << "Day of month:\n\t" << std::flush;
        for (auto const &entry: cron.getDayOfMonthTimes()) {
            auto day = duration_cast<std::chrono::days>(entry);
            std::cout << day.count() + 1 << " "; // start at 0 ?
        }
        std::cout << std::endl;

        std::cout << "Month:\n\t" << std::flush;
        for (auto const &entry: cron.getMonthTimes()) {
            auto day = duration_cast<std::chrono::months>(entry);
            std::cout << day.count() << " ";
        }
        std::cout << std::endl;

        std::cout << "Year:\n\t" << std::flush;
        for (auto const &entry: cron.getYearTimes()) {
            auto day = duration_cast<std::chrono::years>(entry);
            std::cout << day.count() << " ";
        }
        std::cout << std::endl;

        std::cout << "Allowed Weekdays:\n\t" << std::flush;
        for (auto const &entry: cron.getWeekDayTimes()) {
            std::cout << entry << " ";
        }
        std::cout << "\n\n" << std::endl;
    }





}    // namespace CronInterpreter
