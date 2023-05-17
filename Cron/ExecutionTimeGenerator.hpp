//
// Created by Chris on 07.05.2023.
//

#pragma once

#include <iostream>

#include <chrono>
#include <coroutine>
#include <forward_list>
#include <vector>
#include <ranges>
#include "CronInterpreter.hpp"

struct ExecutionTimeGenerator {
    struct promise_type {
        std::vector<std::tm> current_result_times;

        /// Will dont execute the coroutine if the promise_type is not ready
        std::suspend_always initial_suspend() noexcept { return {}; }

        /// Will executed if the coroutine is done
        std::suspend_always final_suspend() noexcept { return {}; }

        ExecutionTimeGenerator get_return_object() noexcept {
            return ExecutionTimeGenerator {std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        /// On co_yield
        std::suspend_always yield_value(std::vector<std::tm> const& value) noexcept {
            current_result_times = value;
            return {};
        }


        /// On co_return
        std::vector<std::tm> return_value(std::vector<std::tm> const& value) noexcept {
            current_result_times = value;
            return {};
        }

        void unhandled_exception() const noexcept {
            std::cout << "unhandled_exception in 'ExecutionTimeGenerator'." << std::endl;
            std::terminate();
        }
    };    // * ExecutionTimeGenerator

    std::coroutine_handle<promise_type> coroutineHandle;

    ExecutionTimeGenerator() : coroutineHandle(nullptr) {}
    explicit ExecutionTimeGenerator(std::coroutine_handle<promise_type> handler) : coroutineHandle(handler) {}

    ~ExecutionTimeGenerator() {
        if(coroutineHandle) {
            coroutineHandle.destroy();
        }
    }

    ExecutionTimeGenerator(const ExecutionTimeGenerator&) = default;
    ExecutionTimeGenerator& operator=(const ExecutionTimeGenerator&) = default;

    ExecutionTimeGenerator(ExecutionTimeGenerator&& other) noexcept : coroutineHandle(other.coroutineHandle) {
        other.coroutineHandle = nullptr;
    }

    ExecutionTimeGenerator& operator=(ExecutionTimeGenerator&& other) noexcept {
        coroutineHandle = other.coroutineHandle;
        other.coroutineHandle = nullptr;
        return *this;
    }

    std::vector<std::tm> operator()() const {
        coroutineHandle.resume();
        return coroutineHandle.promise().current_result_times;
    }


    // ! TODO: fix the input format of the parameters before they get in the function
    static auto generate_from(std::forward_list<std::chrono::seconds> seconds,
                              std::forward_list<std::chrono::seconds> minutes,
                              std::forward_list<std::chrono::seconds> hours,
                              std::forward_list<std::chrono::seconds> daysOfMonth,
                              std::forward_list<std::chrono::seconds> months,
                              std::forward_list<std::chrono::seconds> years,
                              std::vector<int> const& weekdays) ->  ExecutionTimeGenerator {


        auto current_tm = std::tm();
        auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        localtime_s(&current_tm, &current_time);


        std::vector<std::tm> temp = {current_tm};
        CronInterpreter::pretty_print(temp);

        std::vector<std::tm> resultTime;

        year_loop:
        for(auto const& year: years) {
            const auto yearVal = std::chrono::duration_cast<std::chrono::years>(year).count();

            if(yearVal < current_tm.tm_year) {
                continue;
            }

            for(auto const& month: months) {
                const auto monthVal = std::chrono::duration_cast<std::chrono::months>(month).count();

//                std::cout << "month-test " << std::endl;
                if(monthVal < current_tm.tm_mon) {
                    continue;
                }

                for(auto const& dayOfMonth: daysOfMonth) {
                    const auto dayVal = std::chrono::duration_cast<std::chrono::days>(dayOfMonth).count();

//                    std::cout << "day-test " << std::endl;
                    if(dayVal < current_tm.tm_mday) {
                        continue;
                    }

                    for(auto const& hour: hours) {
                        const auto hourVal = std::chrono::duration_cast<std::chrono::hours>(hour).count();

//                        std::cout << "hour-test: " << hourVal << std::endl;



                        if(hourVal +1 < current_tm.tm_hour) {   // TODO: Where is the +1 coming from?
                            std::cout << "true"  << std::endl;
                            continue;
                        }

                        for(auto const& minute: minutes) {
                            const auto minuteVal = std::chrono::duration_cast<std::chrono::minutes>(minute).count();

//                            std::cout << " -> " << current_tm.tm_min << std::endl;

                            if(minuteVal < current_tm.tm_min) {
                                continue;
                            }
//                            std::cout << "minute-test " << std::endl;


                            for(auto const& second: seconds) {
                                const auto secondVal = second.count();

//                                std::cout << "second-test " << std::endl;

//                                if (secondVal < current_tm.tm_sec) { // ! TODO: fix this???
//                                    continue;
//                                }


                                auto timeStruct = std::tm();
                                timeStruct.tm_sec   = secondVal;
                                timeStruct.tm_min   = minuteVal;
                                timeStruct.tm_hour  = hourVal;
                                timeStruct.tm_mday  = dayVal;
                                timeStruct.tm_mon   = monthVal;
                                timeStruct.tm_year  = yearVal;

//                                std::cout << "day: " << timeStruct.tm_mday << std::endl;
                                if(!contained_in_weekdays(weekdays, timeStruct.tm_wday)) {
                                    continue;
                                }
                                resultTime.push_back(timeStruct);

//                                if(!is_reached(timeStruct)) {
//                                }

                                if(resultTime.size() == 10) {
                                    co_yield resultTime;
                                    resultTime.clear();
                                }
                            }
                        }
                    }
                }
            }
        }

        co_return resultTime;
    }

    auto set_weekday_filter() -> ExecutionTimeGenerator {
        this->weekdays = weekdays;
        return *this;
    }

private:
    std::vector<int> weekdays = {0, 1, 2, 3, 4, 5, 6, 7};


    /// Returns true if the given time_point is reached
    static auto is_reached(std::tm const& time_point) -> bool {
        const auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        const auto currentTimeStruct = *std::localtime(&currentTime);

        return currentTimeStruct.tm_sec >= time_point.tm_sec &&
               currentTimeStruct.tm_min >= time_point.tm_min &&
               currentTimeStruct.tm_hour >= time_point.tm_hour &&
               currentTimeStruct.tm_mday >= time_point.tm_mday &&
               currentTimeStruct.tm_mon >= time_point.tm_mon &&
               currentTimeStruct.tm_year >= time_point.tm_year;
    }

    static auto contained_in_weekdays(const std::vector<int>& weekdays, int weekday) -> bool {
        return std::find(weekdays.begin(), weekdays.end(), weekday) != weekdays.end();
    }

};
