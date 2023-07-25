//
// Created by Christoph Rohde on 24.07.23.
//

#pragma once
#include "I_ExecutionTimer.hpp"
#include <iostream>

struct ActionCapsule {
    std::string name;
    I_ExecutionTimer* execution_timer;
//    std::vector<std::tm> const& execution_times;
    std::function<void(void)> action;

};


/// Example:
/// auto action = ActionCapsule(
///     "action",
///     execution_times,
///     []() -> void {
///         std::cout << "action" << std::endl;
///     }
/// );