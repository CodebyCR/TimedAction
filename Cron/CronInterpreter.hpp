//
// Created by Christoph Rohde on 28.12.22.
//

#pragma once

#include <iostream>
#include <Cron.hpp>
#include <iomanip>
#include <ctime>
#include <chrono>

namespace CronInterpreter {

    /**
     * @brief Prints the cartesian product of the cron object as a table
     * @param cronObject
     */
    auto printCartesianProduct(const std::vector<std::vector<std::chrono::seconds>> &cartesianProduct) {
        std::cout << "\n\nSecond Minute Hour DayOfMonth Month Year" << std::endl;
        for (auto &timeVector: cartesianProduct) {
            std::cout << "Execution: ";
            for (auto &time: timeVector) {
                std::cout << std::setfill('0') << std::setw(2) << time.count() << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    /**
     * @brief Prints the result of the converted cartesian product of the cron object to a time as a table
     * @param cronObject
     */
    auto convertCartesianProduct(const std::vector<std::vector<std::chrono::seconds>> &cartesianProduct) {
        auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch());
        auto yearVal = std::chrono::duration_cast<std::chrono::years>(currentTime);
        std::cout << "Seconds:" << currentTime.count() << std::endl;
        std::cout << "\nYear: " << std::setfill('0') << std::setw(4) << (yearVal.count() + 1970) << std::endl;

        std::cout << "\n\nSecond Minute Hour DayOfMonth Month Year" << std::endl;
        for (auto &timeVector: cartesianProduct) {
            for (auto &time: timeVector) {
                std::cout << std::setfill('0') << std::setw(2) << time.count() << " ";

            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }


    auto cartesian_product(Cron const &cronObject) {
        const std::forward_list<std::chrono::seconds> secondTimes = cronObject.getSecondTimes();
        const std::forward_list<std::chrono::seconds> minuteTimes = cronObject.getMinuteTimes();
        const std::forward_list<std::chrono::seconds> hourTimes = cronObject.getHourTimes();
        const std::forward_list<std::chrono::seconds> dayOfMonthTimes = cronObject.getDayOfMonthTimes();
        const std::forward_list<std::chrono::seconds> monthTimes = cronObject.getMonthTimes();
        //const std::vector<u_long> weekDayTimes = cronObject.getWeekDayTimes(); has filter function
        const std::forward_list<std::chrono::seconds> yearTimes = cronObject.getYearTimes();


//        std::vector<std::vector<std::chrono::seconds>> result; // to list from time points
        std::vector<std::tm> resultTime; // to list from time points

        for (auto &year: yearTimes) {

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
                                timeStruct.tm_year = year.count();

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


        for (auto time: cartesianProduct) {
            auto currentTime = std::mktime(&time);
            auto currentTime_ = std::chrono::system_clock::from_time_t(currentTime);

            const auto timeDifferance = std::chrono::duration_cast<std::chrono::seconds>(currentTime_ - now).count();

            if (timeDifferance > 0) {
                result.push_back(time);
            }
        }

        return result;
    }

    static auto filteredOfWeekdayPart(const std::vector<std::tm> &times,
                                      const std::forward_list<std::chrono::seconds> &weekDayTimes) {

        std::vector<std::tm> result;

        for (auto time: times) {
            auto currentTime = std::mktime(&time);
            auto weekday = std::localtime(&currentTime)->tm_wday;
            if (weekday == 0 || weekday == 6) {
                result.push_back(time);
            }
        }

        return result;
    }

    static auto get_time_points(Cron const &cronObject) -> std::string {
        auto cartesianProduct = cartesian_product(cronObject);
        auto filteredOfReachedTimes = filterOfReachedTimes(cartesianProduct);
        auto totalTimes = filteredOfWeekdayPart(filteredOfReachedTimes, cronObject.getWeekDayTimes());


        std::cout << "Size: " << filteredOfReachedTimes.size() << std::endl;

        auto ss = std::stringstream();

        ss << "\n\nHour Minute Second DayOfMonth Month Year" << std::endl;
        for (auto &timeStruct: filteredOfReachedTimes) {
            ss << std::setfill(' ') << std::setw(4) << timeStruct.tm_hour << " "
               << std::setfill(' ') << std::setw(6) << timeStruct.tm_min << " "
               << std::setfill(' ') << std::setw(6) << timeStruct.tm_sec << " "
               << std::setfill(' ') << std::setw(10) << timeStruct.tm_mday << " "
               << std::setfill(' ') << std::setw(5) << timeStruct.tm_mon << " "
               << std::setfill(' ') << std::setw(4) << timeStruct.tm_year
               << std::endl;
        }

        return ss.str();
    }


    auto to_date_time(std::vector<std::chrono::seconds> const &timeVector) {
        std::tm tm = {};
        tm.tm_sec = timeVector[0].count();
        tm.tm_min = timeVector[1].count();
        tm.tm_hour = timeVector[2].count();
        tm.tm_mday = timeVector[3].count();
        tm.tm_mon = timeVector[4].count();
        tm.tm_year = timeVector[5].count();
        tm.tm_isdst = -1;
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    auto to_date_time(std::vector<std::vector<std::chrono::seconds>> const &timeVector) {
        std::vector<std::chrono::system_clock::time_point> result;
        for (const auto &x: timeVector) {
            auto date_time = to_date_time(x);
            std::cout << "dateTime: " << std::ctime(reinterpret_cast<const time_t *>(&date_time));
            result.push_back(date_time);
        }
        return result;
    }

    auto filter(Cron const &cronObject, std::vector<std::chrono::system_clock::time_point> const &timePoints) {
        std::vector<std::vector<std::chrono::seconds>> result;

        // filter out all invalid timePoints dates
//        for (const auto& x : timePoints)
//        {
//            std::tm tm = *std::localtime(&std::chrono::system_clock::to_time_t(x));
//            if (cronObject.getWeekDayTimes().contains(tm.tm_wday))
//            {
//                result.emplace_back(std::vector<std::chrono::seconds>{std::chrono::seconds(tm.tm_sec),
//                                                                      std::chrono::seconds(tm.tm_min),
//                                                                      std::chrono::seconds(tm.tm_hour),
//                                                                      std::chrono::seconds(tm.tm_mday),
//                                                                      std::chrono::seconds(tm.tm_mon),
//                                                                      std::chrono::seconds(tm.tm_year)});
//            }
//        }

        return result;
    }

}
