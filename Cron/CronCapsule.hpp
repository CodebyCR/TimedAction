//
// Created by Christoph Rohde on 11.01.23.
//

#pragma once

#include <iostream>
#include <string>

/** For designated initializer cron expression */
struct CronCapsule {
    std::string second;
    std::string minute;
    std::string hour;
    std::string dayOfMonth;
    std::string month;
    std::string weekday;
    std::string year;
};