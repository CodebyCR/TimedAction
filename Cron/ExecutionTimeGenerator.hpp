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
    auto generate_from(std::forward_list<std::chrono::seconds> seconds,
                              std::forward_list<std::chrono::seconds> minutes,
                              std::forward_list<std::chrono::seconds> hours,
                              std::forward_list<std::chrono::seconds> daysOfMonth,
                              std::forward_list<std::chrono::seconds> months,
                              std::forward_list<std::chrono::seconds> years) -> ExecutionTimeGenerator {

        std::vector<std::tm> resultTime;

        for(auto const& year: years) {
            auto yearVal = std::chrono::duration_cast<std::chrono::years>(year);

            for(auto const& month: months) {
                auto monthVal = std::chrono::duration_cast<std::chrono::months>(month);

                for(auto const& dayOfMonth: daysOfMonth) {
                    auto dayVal = std::chrono::duration_cast<std::chrono::days>(dayOfMonth);

                    for(auto const& hour: hours) {
                        auto hourVal = std::chrono::duration_cast<std::chrono::hours>(hour);

                        for(auto const& minute: minutes) {
                            auto minuteVal = std::chrono::duration_cast<std::chrono::minutes>(minute);

                            for(auto const& second: seconds) {
                                auto timeStruct = std::tm();
                                timeStruct.tm_sec   = second.count();
                                timeStruct.tm_min   = minuteVal.count();
                                timeStruct.tm_hour  = hourVal.count();
                                timeStruct.tm_mday  = dayVal.count();
                                timeStruct.tm_mon   = monthVal.count();
                                timeStruct.tm_year  = yearVal.count();

                                const int weekday = timeStruct.tm_wday;

                                if(!contained_in_weekdays(weekday)) {
                                    continue;
                                }

                                if(!is_reached(timeStruct)) {
                                    resultTime.push_back(timeStruct);
                                }

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

    auto set_weekday_filter(std::vector<int> weekdays) -> ExecutionTimeGenerator {
//        coroutineHandle.promise().current_result_times = filteredOfWeekdayPart(coroutineHandle.promise().current_result_times, weekdays);
        return *this;
    }

private:
    [[maybe_unused]] std::vector<int> weekdays = {0, 1, 2, 3, 4, 5, 6, 7};


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

    auto contained_in_weekdays(const int weekday) -> bool {
        return std::find(weekdays.begin(), weekdays.end(), weekday) != weekdays.end();
    }

};
