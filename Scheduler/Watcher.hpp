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
    std::chrono::milliseconds watchInterval = std::chrono::milliseconds(1000);

public:
    constexpr static std::string_view uninit_time_table = "[ Watcher | ERROR ] -> uninitialised TimeTable.";
    constexpr static std::string_view watch_sleep       = "[ Watcher | SLEEP ] -> No Jobs Found.";

    /// ! for std::format
    /// constexpr static std::string_view found_jobs = "[ Watcher | FOUND ] -> {0} found.";
    /// constexpr static std::string_view watcher_checking = "[ Watcher | CHECKING ] -> {0} for execution at {1}.";
    /// and so on...


    constexpr static unsigned long trigger_sleep_message = 300;

    bool isRunning = true;
    inline static long empty_table_counter = 300;


    [[nodiscard]]
    auto getThread(const std::shared_ptr<TimeTable>& timeTable_ptr) const -> std::thread {
        if(!timeTable_ptr) {
            std::cout << uninit_time_table << std::endl;
        }

        return std::thread([&] {
            while(isRunning) {
                std::this_thread::sleep_for(watchInterval);

                if(timeTable_ptr->empty()) {
                    if(empty_table_counter++ > trigger_sleep_message) {
                        std::cout << watch_sleep << std::endl;
                        empty_table_counter = 0;
                    }
                    continue;
                }


                const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto time_t_vec = timeTable_ptr->get(now);

                std::cout << "[ Watcher | CHECKING ] -> " << time_t_vec.size() << " found." << std::endl;



                /// check if jobs for execution
                std::ranges::for_each(time_t_vec, [&](I_TimedAction*& time_t) {
                    const auto asc_t = std::asctime(std::localtime(&now));
                    std::cout << "[ Watcher | FOUND ] -> " << time_t->getName() << " for execution at " << asc_t << std::endl;
                });


                /// check if jobs for finished execution
                timeTable_ptr->check_status(time_t_vec, now);

            }
        });
    }



    auto set_attributes(std::map<std::string, std::string> attributes) -> void {
        watchInterval = std::chrono::milliseconds(std::stoi(attributes["Watch Interval"]));
    }

};
