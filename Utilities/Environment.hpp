//
// Created by Chris on 13.07.2023.
//

#pragma once

#include <iostream>
#include <string_view>
#include "ConfigJSON.hpp"

constexpr std::string_view SCHEDULER = "Scheduler";
constexpr std::string_view NOTIFICATION = "Notification";
constexpr std::string_view LOG = "Log";

/**
 * This class is a singleton.
 * It contains all the information about the environment.
 * It is used to configure the scheduler and the notification system.
 */
class Environment {
public:
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;
    Environment& operator=(Environment&&) = delete;
    ~Environment() = default;

    friend auto operator << (std::ostream& os, Environment& env) -> std::ostream& {
        os << env.json.to_string();
        return os;
    }

    ///// STATIC METHODS /////

    static std::shared_ptr<Environment> getInstance(std::string_view input) {
        if (!instance) {
            instance = std::make_shared<Environment>( input);
        }
        return instance;
    }

//    static auto get_scheduler_attributes(Environment& env) -> std::optional<std::map<std::string, std::string>> {
//        return instance
//            ? env.json.get_optional_map(SCHEDULER)
//            : std::nullopt;
//    }
//
//    static auto get_notification_attributes(Environment& env) -> std::optional<std::map<std::string, std::string>> {
//        return instance
//            ? env.json.get_optional_map(NOTIFICATION)
//            : std::nullopt;
//    }

    explicit Environment(std::string_view input) : json(input) {}

private:
    static std::shared_ptr<Environment> instance;
    ConfigJSON json;

};
