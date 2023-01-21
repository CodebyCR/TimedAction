//
// Created by Christoph Rohde on 11.01.23.
//

#pragma once

#include <iostream>
#include <string>

/** For designated initializer cron expression */
struct CronCapsule {
    const std::string second;
    const std::string minute;
    const std::string hour;
    const std::string dayOfMonth;
    const std::string month;
    const std::string weekday;
    const std::string year;
};