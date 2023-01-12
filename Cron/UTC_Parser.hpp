//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <../StringUtils.hpp>

auto cron_to_utc(const std::string &cron_string) -> std::string {
    // Split the Cron string into its component parts
    auto cron_parts = StringUtils::split_by(cron_string, ' ');
    std::stringstream cron_stream(cron_string);
    std::string part;
    while (std::getline(cron_stream, part, ' ')) {
        cron_parts.push_back(part);
    }

    // Extract the minute, hour, and day of month values from the Cron string
    int minute = std::stoi(cron_parts[0]);
    int hour = std::stoi(cron_parts[1]);
    int day_of_month = std::stoi(cron_parts[2]);

    // Get the current year and month
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm const *now_tm = std::gmtime(&now_time);
    int year = now_tm->tm_year + 1900;
    int month = now_tm->tm_mon + 1;

    // Create a tm object for the specified time in the current year and month
    std::tm scheduled_tm{};
    scheduled_tm.tm_year = year - 1900;
    scheduled_tm.tm_mon = month - 1;
    scheduled_tm.tm_mday = day_of_month;
    scheduled_tm.tm_hour = hour;
    scheduled_tm.tm_min = minute;
    scheduled_tm.tm_sec = 0;
    scheduled_tm.tm_isdst = -1;

    // Convert the tm object to a time_point in UTC
    std::time_t scheduled_time = std::mktime(&scheduled_tm);
    std::chrono::system_clock::time_point scheduled_time_point = std::chrono::system_clock::from_time_t(scheduled_time);
    auto scheduled_time_utc = std::chrono::system_clock::to_time_t(scheduled_time_point);

    // Return the scheduled time in UTC as a string
    std::stringstream utc_stream;
    utc_stream << std::put_time(std::gmtime(&scheduled_time_utc), "%Y-%m-%d %H:%M:%S %Z");
    return utc_stream.str();
}
