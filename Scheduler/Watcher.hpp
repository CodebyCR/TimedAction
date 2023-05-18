//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include "../Notification/Notification.hpp"
#include "TimeTable.hpp"
#include <future>
#include <iostream>
#include <memory>




class Watcher {
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


    [[nodiscard]] auto getThread(const std::shared_ptr<TimeTable>& timeTable_ptr) const -> std::thread {
        if(!timeTable_ptr) {
            std::cout << uninit_time_table << std::endl;
        }

        return std::thread([&] {
            while(isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                if(timeTable_ptr->empty()) {
                    if(empty_table_counter++ > trigger_sleep_message) {
                        std::cout << watch_sleep << std::endl;
                        empty_table_counter = 0;
                    }
                    continue;
                }


                const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto time_t_vec = timeTable_ptr->get(now);

                std::cout << "[ Watcher | CHECKING ] -> " << time_t_vec.size() << " found." << std::endl;



                /// check if jobs for execution
                std::ranges::for_each(time_t_vec, [&](I_TimedAction*& time_t) {
                    const auto asc_t = std::asctime(std::localtime(&now));
                    std::cout << "[ Watcher | FOUND ] -> " << time_t->getName() << " for execution at " << asc_t << std::endl;
                });


                /// check if jobs for finished execution
                std::ranges::for_each(time_t_vec, [&](I_TimedAction*& time_t) {
                    std::future<Notification> task = time_t->finished();

                    switch(const auto result = task.wait_for(std::chrono::milliseconds(10)); result) {
                        case std::future_status::ready: {
                            std::cout << "[ Watcher ] -> finished " << time_t->getName() << std::endl;
                            // start remove execution time from job here...
                            timeTable_ptr->remove(now);
                        } break;

                        case std::future_status::deferred: {
                            std::cout << "[ Watcher ] -> deferred for " << time_t->getName() << std::endl;
                        } break;

                        case std::future_status::timeout: {
                            std::cout << "[ Watcher ] -> timeout for " << time_t->getName() << std::endl;
                            timeTable_ptr->remove(now);
                        } break;

                        default: {
                            std::cout << "[ Watcher | ERROR ] -> unknown status of " << time_t->getName() << std::endl;
                        }

                    }
                });
            }
        });
    }
};
