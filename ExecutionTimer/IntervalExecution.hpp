//
// Created by Chris on 01.06.2023.
//

#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include "../Interfaces/I_ExecutionTimer.hpp"

class IntervalExecution : public std::chrono::milliseconds, I_ExecutionTimer {
public:
    IntervalExecution() = default;

    IntervalExecution(std::chrono::milliseconds &interval) {
        this->operator=(interval);
    }

    IntervalExecution(std::chrono::milliseconds &&interval) {
        this->operator=(interval);
    }

    [[nodiscard]] auto get_execution_times() -> std::vector<std::tm> override {
        std::cout << "NOT IMPLEMENTED YET!" << std::endl;
        return {};
    }
};
