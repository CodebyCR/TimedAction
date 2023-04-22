//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include <iostream>
#include <memory>
#include "EventQueue.hpp"




class Watcher {     // maybe functor?

public:

    bool isRunning = false;

    [[nodiscard]]
    auto getThread(const std::shared_ptr<EventQueue>& eventQueue_ptr) const -> std::thread
    {
        return std::thread([&] {
            while (isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                std::cout << "Watcher: " << std::this_thread::get_id() << std::endl;

                // TODO: compare current time with execution times

                eventQueue_ptr->pop();

            }
        });
    }


};
