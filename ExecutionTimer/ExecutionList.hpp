//
// Created by Chris on 01.06.2023.
//

#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include "../Interfaces/I_ExecutionTimer.hpp"

class ExecutionList : public std::vector<std::tm>, I_ExecutionTimer {
public:
    ExecutionList() = default;

    ExecutionList(std::vector<std::tm> &execution_times) {
        this->insert(this->begin(), execution_times.begin(), execution_times.end());
    }

    ExecutionList(std::vector<std::tm> &&execution_times) {
        this->insert(this->begin(), execution_times.begin(), execution_times.end());
    }

    ExecutionList(std::initializer_list<std::tm> execution_times) {
        this->insert(this->begin(), execution_times.begin(), execution_times.end());
    }

    [[nodiscard]] auto get_execution_times() -> std::vector<std::tm> override {
        return *this;
    }
};
