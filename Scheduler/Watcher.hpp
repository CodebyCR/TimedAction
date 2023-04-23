//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include <iostream>
#include <memory>
#include "JobMap.hpp"




class Watcher {     // maybe functor?

public:

    bool isRunning = false;

    [[nodiscard]]
    auto getThread(const std::shared_ptr<JobMap>& jobMap_ptr) const -> std::thread
    {
        return std::thread([&] {
            while (isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));


                const auto now = std::time(nullptr);

                auto time_t_vec = jobMap_ptr->get(now);

                std::ranges::for_each(time_t_vec, [&](auto &time_t) {
                    const auto asc_t = std::asctime(std::localtime(&now));
                    std::cout << "Watcher: found " << time_t->getName() << " for execution at " << asc_t << std::endl;
                    time_t->start();
                    jobMap_ptr->remove(now);
                });

            }
        });
    }


};
