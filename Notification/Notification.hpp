//
// Created by Christoph Rohde on 24.04.23.
//

#pragma once

#include <iostream>

/// ABSTRACT CLASS
class Notification {
protected:
    std::string_view name;
    std::string_view execution_time;

public:

    virtual ~Notification() = default;

    virtual auto getName() const -> std::string_view
    {
        return name;
    }

    virtual auto get_execution_time() const -> std::string_view
    {
        return execution_time;
    }

    virtual auto set_execution_time(std::string_view execution_time) -> void
    {
        this->execution_time = execution_time;
    }

};
