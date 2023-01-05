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
    auto printCartesianProduct(const std::vector<std::vector<std::chrono::seconds>> &cartesianProduct)
    {
        std::cout << "\n\nSecond Minute Hour DayOfMonth Month Year" << std::endl;
        for (auto &timeVector : cartesianProduct) {
            std::cout << "Execution: ";
            for (auto &time : timeVector) {
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
    auto convertCartesianProduct(const std::vector<std::vector<std::chrono::seconds>> &cartesianProduct)
    {
        auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
        auto yearVal = std::chrono::duration_cast<std::chrono::years>(currentTime);
        std::cout <<"Seconds:" << currentTime.count() << std::endl;
        std::cout << "\nYear: " << std::setfill('0') << std::setw(4) << (yearVal.count() + 1970) << std::endl;

        std::cout << "\n\nSecond Minute Hour DayOfMonth Month Year" << std::endl;
        for (auto &timeVector : cartesianProduct) {
            for (auto &time : timeVector) {
                std::cout << std::setfill('0') << std::setw(2) << time.count() << " ";

            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }


    auto cartesian_product(Cron const& cronObject)
    {
        const std::forward_list<std::chrono::seconds> secondTimes = cronObject.getSecondTimes();
        const std::forward_list<std::chrono::seconds> minuteTimes = cronObject.getMinuteTimes();
        const std::forward_list<std::chrono::seconds> hourTimes = cronObject.getHourTimes();
        const std::forward_list<std::chrono::seconds> dayOfMonthTimes = cronObject.getDayOfMonthTimes();
        const std::forward_list<std::chrono::seconds> monthTimes = cronObject.getMonthTimes();
        //const std::vector<u_long> weekDayTimes = cronObject.getWeekDayTimes(); has filter function
        const std::forward_list<std::chrono::seconds> yearTimes = cronObject.getYearTimes();


        for (auto year : yearTimes) {
            // print year
            std::cout << std::setfill('0') << std::setw(4) << year.count() << " ";
        }

        std::vector<std::vector<std::chrono::seconds>> result;

        for (auto &second : secondTimes) {
            for (auto &minute : minuteTimes) {
                for (auto &hour : hourTimes) {
                    for (auto &dayOfMonth : dayOfMonthTimes) {
                        auto dayVal = std::chrono::duration_cast<std::chrono::days>(dayOfMonth);
                        std::cout << "\nDay: " << std::setfill('0') << std::setw(4) << dayVal.count() << std::endl;
                        for (auto &month : monthTimes) {
                            for (auto &year : yearTimes) {
                                auto yearVal = std::chrono::duration_cast<std::chrono::years>(year);
//                                std::cout << "\nYear: " << std::setfill('0') << std::setw(4) << yearVal.count() << std::endl;

                                std::vector<std::chrono::seconds> timeVector;
                                timeVector.push_back(second);
                                timeVector.push_back(minute);
                                timeVector.push_back(hour);
                                timeVector.push_back(dayOfMonth);
                                timeVector.push_back(month);
                                timeVector.push_back(year);
                                result.push_back(timeVector);
                            }
                        }
                    }
                }
            }
        }

        convertCartesianProduct(result);
        printCartesianProduct(result);
        return result;
    }





    auto to_date_time(std::vector<std::chrono::seconds> const& timeVector)
    {

        //std::cout << "secunden: " << std::setfill('0') << std::setw(2) << std::to_string(static_cast<long long>(timeVector[4])) << " ";

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

    auto to_date_time(std::vector<std::vector<std::chrono::seconds>> const& timeVector)
    {
        std::vector<std::chrono::system_clock::time_point> result;
        for (const auto& x : timeVector)
        {
            auto date_time = to_date_time(x);
            std::cout << "dateTime: " << std::ctime(reinterpret_cast<const time_t *>(&date_time));
            result.push_back(date_time);
        }
        return result;
    }

    auto filter(Cron const& cronObject, std::vector<std::chrono::system_clock::time_point> const& timePoints)
    {
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



    static auto print_time_points(Cron const& cronObject){
        auto timeMatrix = cartesian_product(cronObject);
        auto timePoints = to_date_time(timeMatrix);

        // timePoint vector to local time
        std::vector<std::tm> localTimePoints;
        for (const auto& x : timePoints)
        {
            auto time = std::chrono::system_clock::to_time_t(x);
            localTimePoints.emplace_back(*std::localtime(&time));
        }

        // print timePoints
        std::cout << " TimePoints size: " << timePoints.size() << std::endl;
        std::cout << " Local TimePoints size: " << localTimePoints.size() << std::endl;
        for (const auto& x : localTimePoints)
        {
            std::cout << std::put_time(&x, "%c") << std::endl;
        }
    }

}
