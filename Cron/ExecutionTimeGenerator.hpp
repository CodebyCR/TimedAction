//
// Created by Chris on 07.05.2023.
//

#pragma once

#include <iostream>

#include <chrono>
#include <coroutine>
#include <forward_list>
#include <vector>

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
            std::cout << "unhandled_exception" << std::endl;
            std::terminate();
        }
    };    // * ExecutionTimeGenerator

    std::coroutine_handle<promise_type> coro;

    explicit ExecutionTimeGenerator(std::coroutine_handle<promise_type> handler) : coro(handler) {}

    ~ExecutionTimeGenerator() {
        if(coro) {
            coro.destroy();
        }
    }

    ExecutionTimeGenerator(const ExecutionTimeGenerator&) = delete;
    ExecutionTimeGenerator& operator=(const ExecutionTimeGenerator&) = delete;

    ExecutionTimeGenerator(ExecutionTimeGenerator&& other) noexcept : coro(other.coro) {
        other.coro = nullptr;
    }

    ExecutionTimeGenerator& operator=(ExecutionTimeGenerator&& other) noexcept {
        coro = other.coro;
        other.coro = nullptr;
        return *this;
    }

    std::vector<std::tm> operator()() const {
        coro.resume();
        return coro.promise().current_result_times;
    }

    static ExecutionTimeGenerator generate_from(std::vector<std::chrono::seconds> seconds,
                                                std::vector<std::chrono::minutes> minutes,
                                                std::vector<std::chrono::hours> hours,
                                                std::vector<std::chrono::days> daysOfMonth,
                                                std::vector<std::chrono::months> months,
                                                std::vector<std::chrono::years> years) {
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
                                timeStruct.tm_sec = second.count();
                                timeStruct.tm_min = minuteVal.count();
                                timeStruct.tm_hour = hourVal.count();
                                timeStruct.tm_mday = dayVal.count();
                                timeStruct.tm_mon = monthVal.count();
                                timeStruct.tm_year = yearVal.count();

                                resultTime.push_back(timeStruct);

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
};
