//
// Created by Christoph Rohde on 28.12.22.
//

#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

namespace CronInterpreter {

    auto get_year_dif_in_sec() -> long long {//Current year as seconds
        auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch());
        auto yearVal = std::chrono::duration_cast<std::chrono::years>(currentTime);
        auto currentYear = yearVal.count() ;

        // 53 as seconds
        auto weekDaySeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::years(53));
        auto count = weekDaySeconds.count();
        return count;
    }

    auto get_month_from_index(const int index){
        auto months = std::vector{"January",
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
        for (auto const &timeStruct: timePoints) {
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

    static auto pretty_print(std::vector<std::tm> &timePoints){
        for (auto &timeStruct: timePoints) {
            std::time_t time = std::mktime(&timeStruct);
            std::cout << std::ctime(&time);
        }
    }

}
