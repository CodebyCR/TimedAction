//
// Created by Christoph Rohde on 28.12.22.
//

#pragma once
#include <iostream>
#include <Cron.hpp>

namespace CronInterpreter {

    auto cartesian_product(Cron const& cronObject)
    {
        const std::vector<u_long> secondTimes = cronObject.getSecondTimes();
        const std::vector<u_long> minuteTimes = cronObject.getMinuteTimes();
        const std::vector<u_long> hourTimes = cronObject.getHourTimes();
        const std::vector<u_long> dayOfMonthTimes = cronObject.getDayOfMonthTimes();
        const std::vector<u_long> monthTimes = cronObject.getMonthTimes();
        //const std::vector<u_long> weekDayTimes = cronObject.getWeekDayTimes(); has filter function
        const std::vector<u_long> yearTimes = cronObject.getYearTimes();

        std::vector<std::vector<u_long>> result;

        for (const auto& x : secondTimes)
        {
            for (const auto& y : minuteTimes)
            {
                for (const auto& z : hourTimes)
                {
                    for (const auto& w : dayOfMonthTimes)
                    {
                        for (const auto& u : monthTimes)
                        {
                            for (const auto& t : yearTimes)
                            {
                                result.emplace_back(std::vector<u_long>{x, y, z, w, u, t});
                            }
                        }
                    }
                }
            }
        }

        return result;
    }
}
