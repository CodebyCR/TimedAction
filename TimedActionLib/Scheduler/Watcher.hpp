//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include "../Interfaces/Message.hpp"
#include "TimeTable.hpp"
#include <future>
#include <iostream>
#include <memory>

class Watcher {
private:
    Dispatcher dispatcher;

    std::chrono::milliseconds watchInterval = std::chrono::milliseconds(
            Dispatcher::to_int_or_else(
                    dispatcher.env_lookup(WATCHER, WATCH_INTERVAL),
                    1'000));

public:
    /// Map Key
    constexpr static std::string_view WATCHER = "Watcher";

    /// Sub Keys
    constexpr static std::string_view WATCH_INTERVAL = "Watch Interval";

    /// STATICS
    constexpr static std::string_view watch_sleep = "[ Watcher | SLEEP ] -> No active Watchables found.";

    /// ! for std::format
    /// constexpr static std::string_view found_jobs = "[ Watcher | FOUND ] -> {0} found.";
    /// constexpr static std::string_view watcher_checking = "[ Watcher | CHECKING ] -> {0} for execution at {1}.";
    /// and so on...

    constexpr static unsigned long trigger_sleep_message = 300;

    bool isRunning = true;
    inline static long empty_table_counter = 300;


    [[nodiscard]]
    auto open_watch_thread(std::vector<std::shared_ptr<Watchable>> const& watchables) const -> std::thread {
        return std::thread([&] {
            while(isRunning) {
                std::this_thread::sleep_for(watchInterval);

                bool inactive_watchables = std::ranges::all_of(watchables, [](auto& watchable) {
                    return watchable->inactive();
                });

                if(inactive_watchables) {
                    if(empty_table_counter++ > trigger_sleep_message) {
                        std::cout << watch_sleep << std::endl;
                        empty_table_counter = 0;
                    }
                    continue;
                }

                const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

                std::ranges::for_each(watchables, [&](auto& watchable) {
                    watchable->watch(now);
                });
            }
        });
    }
};
