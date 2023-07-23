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

    auto get_scheduler_attributes() -> std::map<std::string, std::string> {
            return env->get_attributes(ENV::SCHEDULER)
                ? env->get_attributes(ENV::SCHEDULER).value()
                : std::map<std::string, std::string>(); // add default values
        }

    auto get_notification_attributes() -> std::optional<std::map<std::string, std::string>> {
        return env->get_attributes(ENV::NOTIFICATION);
    }

    auto get_log_attributes() -> std::optional<std::map<std::string, std::string>> {
        return env->get_attributes(ENV::LOG);
    }

    auto get_watcher_attributes() -> std::map<std::string, std::string> {
        return env->get_attributes(ENV::WATCHER)
            ? env->get_attributes(ENV::WATCHER).value()
            : std::map<std::string, std::string>(); // add default values
    }

private:
    std::shared_ptr<ENV::Environment> env;
};

