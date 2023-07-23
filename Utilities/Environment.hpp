//
// Created by Chris on 13.07.2023.
//

#pragma once

#include <iostream>
#include <string_view>
#include "ConfigJSON.hpp"

namespace ENV {
    constexpr std::string_view SCHEDULER = "Scheduler";
    constexpr std::string_view NOTIFICATION = "Notification";
    constexpr std::string_view LOG = "Log";


    /// return a environment variable or an empty string
    static auto get_env_variable(std::string_view const env_variable) -> std::string {
        auto env_value = std::getenv(env_variable.data());
        if (!env_value) {
            std::cout << env_variable << " not found" << std::endl;
            return "";
        }
        return env_value;
    }

    /**
     * This class is a singleton.
     * It contains all the information about the environment.
     * It is used to configure the scheduler and the notification system.
     */
    class Environment {
    public:
        explicit Environment(std::string_view input) : json(input) {}
        Environment(const Environment&) = delete;
        Environment& operator=(const Environment&) = delete;
        Environment& operator=(Environment&&) = delete;
        ~Environment() = default;

        friend auto operator<<(std::ostream& os, Environment& env) -> std::ostream& {
            os << env.json.to_string();
            return os;
        }

        auto get_attributes(std::string_view key) -> std::optional<std::map<std::string, std::string>> {
            return json.get_optional_map(key);
        }

        ///// STATIC METHODS /////

        static std::shared_ptr<Environment> getInstance(std::string_view input) {
            static std::shared_ptr<Environment> instance;
            if(!instance) {
                instance = std::make_shared<Environment>(input);
            }
            return instance;
        }

    private:
        ConfigJSON json;
    };

}
