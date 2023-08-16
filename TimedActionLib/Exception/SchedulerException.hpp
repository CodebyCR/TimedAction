//
// Created by Chris on 16.08.2023.
//

#pragma once

#include <iostream>
class SchedulerException : public std::exception {
private:
    std::string_view errorMessage;

public:
    explicit SchedulerException(std::string_view message): errorMessage(message) {}

    [[nodiscard]]
    const char * what() const noexcept override {
        return errorMessage.data();
    }

    ~SchedulerException() override = default;
};