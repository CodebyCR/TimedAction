//
// Created by Christoph Rohde on 27.07.23.
//

#pragma once

#include <iostream>

/// This is interface defines the methods for a watchable object.
struct Watchable {
    virtual auto watch(std::time_t const& time) -> void = 0;

    virtual auto inactive() -> bool = 0;
};


