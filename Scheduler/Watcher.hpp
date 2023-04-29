//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include <iostream>
#include <memory>
#include <future>
#include "../Notification/Notification.hpp"
#include "TimeTable.hpp"


class Watcher {

public:

    bool isRunning = true;
    std::future<Notification> task; // maybe log instead of void later

    [[nodiscard]]
    auto getThread(const std::shared_ptr<TimeTable>& timeTable_ptr) const -> std::thread
    {

        if(!timeTable_ptr){
            std::cout << "Watcher: timeTable_ptr is null" << std::endl;
        }

        return std::thread([&] {
            while (isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                const auto time_t_vec = timeTable_ptr->get(now);

                std::cout << "Watcher: checking for jobs (" << time_t_vec.size() << " found)" << std::endl;


                /// check if jobs for execution
                std::ranges::for_each(time_t_vec, [&](auto &time_t) {
                    const auto asc_t = std::asctime(std::localtime(&now));
                    std::cout << "Watcher: found " << time_t->getName() << " for execution at " << asc_t << std::endl;
//                    time_t->start();
//                    timeTable_ptr->remove(now);
                });



                /// check if jobs for finished execution
//                std::ranges::for_each(time_t_vec, [&](auto &time_t) {
//                    std::future test = time_t->finished();
//
//                    switch(auto result = task.wait_for(std::chrono::milliseconds(10)); result) {
//                        case std::future_status::deferred: {
//                            std::cout << "Watcher: deferred for " << time_t->getName() << std::endl;
//                        }
//                        break;
//                        case std::future_status::timeout: {
//                            std::cout << "Watcher: timeout for " << time_t->getName() << std::endl;
//                        }
//                        break;
//                        case std::future_status::ready: {
//                            std::cout << "Watcher: finished " << time_t->getName() << std::endl;
//                        }
//                        break;
//                    }
//
//                });


            }
        });
    }


};
