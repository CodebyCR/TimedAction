//
// Created by Chris on 01.06.2023.
//

#pragma once

#include <iostream>
#include <vector>
#include <chrono>

/// <br/><b>I_ExecutionTimer</b><br/>
/// Can used by the TimedActionLib Object to get the execution times.
class I_ExecutionTimer {
public:
    [[nodiscard]]
    virtual auto get_execution_times() -> std::vector<std::tm> = 0;
};

// * for cron, interval and time vector
