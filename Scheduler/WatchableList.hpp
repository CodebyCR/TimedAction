//
// Created by Christoph Rohde on 27.07.23.
//

#pragma once

#include <iostream>
#include "../Interfaces/Watchable.hpp"

class WatchableList: public std::vector<std::shared_ptr<Watchable>> {
public:
    auto add(std::shared_ptr<Watchable> const& watchable) -> void {
        if(!watchable) {
            std::cout << "[ WatchableList | ERROR ] -> uninitialised Watchable." << std::endl;
            return;
        }

        this->emplace_back(watchable);
    }
};
