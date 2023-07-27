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
    std::chrono::milliseconds watchInterval = std::chrono::milliseconds(1'000);

public:
    constexpr static std::string_view watch_sleep       = "[ Watcher | SLEEP ] -> No Jobs Found.";

    /// ! for std::format
    /// constexpr static std::string_view found_jobs = "[ Watcher | FOUND ] -> {0} found.";
    /// constexpr static std::string_view watcher_checking = "[ Watcher | CHECKING ] -> {0} for execution at {1}.";
    /// and so on...


    constexpr static unsigned long trigger_sleep_message = 300;

    bool isRunning = true;
    inline static long empty_table_counter = 300;


    [[nodiscard]]
    auto open_watch_thread(const std::span<std::shared_ptr<Watchable>> watchables) const -> std::thread {

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

                for(auto const& watchable : watchables){
                    watchable->watch(now);
                }


            }
        });
    }



    auto set_attributes(std::map<std::string, std::string> attributes) -> void {
        watchInterval = std::chrono::milliseconds(std::stoi(attributes["Watch Interval"]));
    }

};
