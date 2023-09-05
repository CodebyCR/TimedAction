//
// Created by Christoph Rohde on 04.09.23.
//

#pragma once

#include <../Utilities/Logger.hpp>

#include <iostream>
#include <variant>
#include <functional>


class Action: public std::variant<std::function<void()>, std::function<Logger(Logger)>> {
public:
    explicit Action(std::function<void()> func) : std::variant<std::function<void()>, std::function<Logger(Logger)>>(std::move(func)) {}
    explicit Action(std::function<Logger(Logger)> func) : std::variant<std::function<void()>, std::function<Logger(Logger)>>(std::move(func)) {}
    ~Action() = default;

    // state with switch
    auto execute() -> Logger {

    }
};
