//
// Created by Christoph Rohde on 28.12.22.
//

#pragma once

#include <iostream>
#include "Cron.hpp"
#include <iomanip>
#include <ctime>
#include <chrono>
#include <algorithm>

namespace CronInterpreter {

//    /**
//     * @brief Prints the cartesian product of the cron object as a table
//     * @param cronObject
//     */
//    auto printCartesianProduct(const std::vector<std::vector<std::chrono::seconds>> &cartesianProduct) {
//        std::cout << "\n\nSecond Minute Hour DayOfMonth Month Year" << std::endl;
//        for (auto &timeVector: cartesianProduct) {
//            std::cout << "Execution: ";
//            for (auto &time: timeVector) {
//                std::cout << std::setfill('0') << std::setw(2) << time.count() << " ";
//            }
//            std::cout << std::endl;
//        }
//        std::cout << std::endl;
//    }
//
//    /**
//     * @brief Prints the result of the converted cartesian product of the cron object to a time as a table
//     * @param cronObject
//     */
//    auto convertCartesianProduct(const std::vector<std::vector<std::chrono::seconds>> &cartesianProduct) {
//        auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(
//                std::chrono::system_clock::now().time_since_epoch());
//        auto yearVal = std::chrono::duration_cast<std::chrono::years>(currentTime);
//        std::cout << "Seconds:" << currentTime.count() << std::endl;
//        std::cout << "\nYear: " << std::setfill('0') << std::setw(4) << (yearVal.count() + 1970) << std::endl;
//
//        std::cout << "\n\nSecond Minute Hour DayOfMonth Month Year" << std::endl;
//        for (auto &timeVector: cartesianProduct) {
//            for (auto &time: timeVector) {
//                std::cout << std::setfill('0') << std::setw(2) << time.count() << " ";
//
//            }
//            std::cout << std::endl;
//        }
//        std::cout << std::endl;
//    }

    static auto sortByNextReachedTime(std::vector<std::tm> &times) {
        std::sort(times.begin(), times.end(), []( std::tm &a,  std::tm &b) {
            time_t timeA = std::mktime(&a);
            time_t timeB = std::mktime(&b);
            return difftime(timeA, timeB) < 0;
        });

    }

    auto get_year_dif_in_sec() -> long long {//Current year as seconds
        auto currentTime = std::__1::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch());
        auto yearVal = std::__1::chrono::duration_cast<std::chrono::years>(currentTime);
        auto currentYear = yearVal.count() ;

        // 53 as seconds
        auto weekDaySeconds = std::__1::chrono::duration_cast<std::chrono::seconds>(std::chrono::years(53));
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

    auto cartesian_product(Cron const &cronObject) {
        const std::forward_list<std::chrono::seconds> secondTimes = cronObject.getSecondTimes();
        const std::forward_list<std::chrono::seconds> minuteTimes = cronObject.getMinuteTimes();
        const std::forward_list<std::chrono::seconds> hourTimes = cronObject.getHourTimes();
        const std::forward_list<std::chrono::seconds> dayOfMonthTimes = cronObject.getDayOfMonthTimes();
        const std::forward_list<std::chrono::seconds> monthTimes = cronObject.getMonthTimes();
        const std::forward_list<std::chrono::seconds> yearTimes = cronObject.getYearTimes();


        std::vector<std::tm> resultTime; // to list from time points

        for (auto &year: yearTimes) {
            auto yearVal = std::chrono::duration_cast<std::chrono::years>(year);

            for (auto &month: monthTimes) {
                auto monthVal = std::chrono::duration_cast<std::chrono::months>(month);

                for (auto &dayOfMonth: dayOfMonthTimes) {
                    auto dayVal = std::chrono::duration_cast<std::chrono::days>(dayOfMonth);

                    for (auto &hour: hourTimes) {
                        auto hourVal = std::chrono::duration_cast<std::chrono::hours>(hour);

                        for (auto &minute: minuteTimes) {
                            auto minuteVal = std::chrono::duration_cast<std::chrono::minutes>(minute);

                            for (auto &second: secondTimes) {

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



    static auto filterOfReachedTimes(const std::vector<std::tm> &cartesianProduct) {
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

    static auto filteredOfWeekdayPart(const std::vector<std::tm> &times,
                                      const std::vector<int> &weekdays) {

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

    static auto get_info(std::vector<std::tm> &timePoints) -> std::string {
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

    static auto get_time_points(Cron const &cronObject) -> std::vector<std::tm> {
        auto cartesianProduct = cartesian_product(cronObject);
        auto filteredOfReachedTimes = filterOfReachedTimes(cartesianProduct);
        auto totalTimes = filteredOfWeekdayPart(filteredOfReachedTimes, cronObject.getWeekDayTimes());

        sortByNextReachedTime(totalTimes);

        return totalTimes;
    }

}
