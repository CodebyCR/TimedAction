//
// Created by Christoph Rohde on 11.01.23.
//

#pragma once

#include <iostream>
#include <utility>

/** For designated initializer cron expression */
struct CronCapsule {
    /** The second field (is optional) */
    const std::string second;

    /** The minute field */
    const std::string minute;

    /** The hour field */
    const std::string hour;

    /** The day of month field */
    const std::string dayOfMonth;

    /** The month field */
    const std::string month;

    /** The weekday field */
    const std::string weekday;

    /** The year field */
    const std::string year;
};