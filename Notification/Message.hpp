//
// Created by Christoph Rohde on 24.04.23.
//

#pragma once

#include <iostream>

/// ABSTRACT CLASS
class Message {
protected:
    std::string_view name;
    std::string_view execution_time;

public:
    explicit Message(std::string_view name, std::string_view execution_time)
        : name(name), execution_time(execution_time) {}

    virtual ~Message() = default;



    [[nodiscard]]
    virtual auto getName() const -> std::string_view {
        return name;
    }

    [[nodiscard]]
    auto get_execution_time() const -> std::string_view {
        return execution_time;
    }

};
