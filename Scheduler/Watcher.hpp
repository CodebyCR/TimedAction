//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include <iostream>
#include <memory>
#include <future>
#include "JobMap.hpp"
#include "../Notification/Notification.hpp"


class Watcher {

public:

    bool isRunning = true;
    std::future<Notification> task; // maybe log instead of void later

    [[nodiscard]]
    auto getThread(const std::shared_ptr<JobMap>& jobMap_ptr) const -> std::thread
    {
        return std::thread([&] {
            while (isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                std::cout << "Watcher: checking for jobs" << std::endl;

                auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

                // std::cout << "-- 3.4" << std::endl;

                try {
                    // ! TODO: fix this
                    // auto time_t_vec = jobMap_ptr->get(now);
                } catch (std::exception& e) {
                    std::cout << "Watcher: " << e.what() << std::endl;
                }

                // std::cout << "-- 3.5" << std::endl;

                /// check if jobs for execution
//                std::ranges::for_each(time_t_vec, [&](auto &time_t) {
//                    const auto asc_t = std::asctime(std::localtime(&now));
//                    std::cout << "Watcher: found " << time_t->getName() << " for execution at " << asc_t << std::endl;
//                    time_t->start();
//                    jobMap_ptr->remove(now);
//                });



//                /// check if jobs for finished execution
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
