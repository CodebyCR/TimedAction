//
// Created by Christoph Rohde on 23.07.23.
//

#pragma once

#include "Environment.hpp"

class Dispatcher{
public:

    Dispatcher(){
        const std::string scheduler_json = ENV::get_env_variable("Scheduler_json");
        env = ENV::Environment::getInstance(scheduler_json);
    }

    [[nodiscard]]
    auto env_lookup(std::string_view map_key, std::string_view sub_key) -> std::optional<std::string> {
        return env->lookup(map_key, sub_key);
    }

    static auto to_bool_or_else(std::optional<std::string> const& value, bool default_value = false) -> bool {
        return value
               ? value.value() == "true"
               : default_value;
    }

    static auto to_int_or_else(std::optional<std::string> const& value, int default_value = 0) -> int {
        return value
               ? std::stoi(value.value())
               : default_value;
    }

private:
    std::shared_ptr<ENV::Environment> env;
};

