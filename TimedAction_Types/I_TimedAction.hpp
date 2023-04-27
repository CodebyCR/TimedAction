//
// Created by Christoph Rohde on 13.12.22.
//

#pragma once

#include <iostream>
#include <future>
#include "../Cron/Cron.hpp"
#include "../Notification/Notification.hpp"

class I_TimedAction {

public:

    virtual ~ I_TimedAction() = default;

    //virtual auto finished() -> std::future<Notification> = 0;

    virtual auto start() -> void = 0;

    virtual auto stop() -> void = 0;

    virtual auto restart() -> void = 0;

    [[nodiscard]]
    virtual auto is_running() const -> bool = 0;

    [[nodiscard]]
    virtual auto getName() const -> std::string_view = 0;

    [[nodiscard]]
    virtual auto get_execution_times() const -> std::vector<std::tm> = 0;


};
