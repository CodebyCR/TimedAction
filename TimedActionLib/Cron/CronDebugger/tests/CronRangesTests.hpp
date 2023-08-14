//
// Created by Christoph Rohde on 09.06.23.
//

#pragma once
#include "../src/CronRanges.hpp"
#include <iostream>


namespace CronTest {

    auto second_ranges_test() -> bool {
        auto ranges = std::vector<std::string>();
        ranges.emplace_back("0-59");      // true
        ranges.emplace_back("13-29");     // true
        ranges.emplace_back("5-19");      // true
        ranges.emplace_back("0-60");      // false
        ranges.emplace_back("0-0");       // false
        ranges.emplace_back("59-59");     // false
        ranges.emplace_back("0-5-10");    // false
        ranges.emplace_back("59");        // true
        ranges.emplace_back("12");        // true

        for(int index = 0; index < ranges.size(); index++) {
            bool result = CronRanges::second_range_matched(ranges[index]);

            if(index > 2) {
                // test that should be false
                result = !result;
            }

            if(!result) {
                std::cout << "second range test " << (index + 1) << ": "
                          << "failed !" << std::endl;
                return false;
            }

            std::cout << "second range test " << (index + 1) << ": "
                      << "passed." << std::endl;
        }

        std::cout << "All second range tests passed.\n"
                  << std::endl;
        return true;
    }

    auto hour_ranges_test() -> bool {
        auto ranges = std::vector<std::string>();
        ranges.emplace_back("0-23");      // true
        ranges.emplace_back("13-23");     // true
        ranges.emplace_back("5-19");      // true
        ranges.emplace_back("0-24");      // false
        ranges.emplace_back("0-0");       // false
        ranges.emplace_back("23-23");     // false
        ranges.emplace_back("0-5-10");    // false
        ranges.emplace_back("23");        // false
        ranges.emplace_back("12");        // false

        for(int index = 0; index < ranges.size(); index++) {
            bool result = CronRanges::hour_range_matched(ranges[index]);

            if(index > 2) {
                // test that should be false
                result = !result;
            }

            if(!result) {
                std::cout << "hour range test " << (index + 1) << ": "
                          << "failed !" << std::endl;
                return false;
            }

            std::cout << "hour range test " << (index + 1) << ": "
                      << "passed." << std::endl;
        }

        std::cout << "All hour range tests passed.\n"
                  << std::endl;
        return true;
    }

    auto day_ranges_test() -> bool {
        auto ranges = std::vector<std::string>();
        ranges.emplace_back("1-31");      // true
        ranges.emplace_back("13-31");     // true
        ranges.emplace_back("5-19");      // true
        ranges.emplace_back("0-32");      // false
        ranges.emplace_back("0-0");       // false
        ranges.emplace_back("0-31");      // false
        ranges.emplace_back("0-5-10");    // false
        ranges.emplace_back("31");        // false
        ranges.emplace_back("12");        // false

        for(int index = 0; index < ranges.size(); index++) {
            bool result = CronRanges::day_range_matched(ranges[index]);

            if(index > 2) {
                // test that should be false
                result = !result;
            }

            if(!result) {
                std::cout << "day range test " << (index + 1) << ": "
                          << "failed !" << std::endl;
                return false;
            }

            std::cout << "day range test " << (index + 1) << ": "
                      << "passed." << std::endl;
        }

        std::cout << "All day range tests passed.\n"
                  << std::endl;
        return true;
    }

    auto month_ranges_test() -> bool {
        auto ranges = std::vector<std::string>();
        ranges.emplace_back("1-12");      // true
        ranges.emplace_back("2-7");       // true
        ranges.emplace_back("5-11");      // true
        ranges.emplace_back("0-13");      // false
        ranges.emplace_back("0-0");       // false
        ranges.emplace_back("0-12");      // false
        ranges.emplace_back("0-5-10");    // false
        ranges.emplace_back("12");        // false
        ranges.emplace_back("7");         // false

        for(int index = 0; index < ranges.size(); index++) {
            bool result = CronRanges::month_range_matched(ranges[index]);

            if(index > 2) {
                // test that should be false
                result = !result;
            }

            if(!result) {
                std::cout << "month range test " << (index + 1) << ": "
                          << "failed !" << std::endl;
                return false;
            }

            std::cout << "month range test " << (index + 1) << ": "
                      << "passed." << std::endl;
        }

        std::cout << "All month range tests passed.\n"
                  << std::endl;
        return true;
    }

    auto weekday_ranges_test() -> bool {
        auto ranges = std::vector<std::string>();
        ranges.emplace_back("0-6");       // true
        ranges.emplace_back("1-7");       // true
        ranges.emplace_back("SUN-SAT");   // true
        ranges.emplace_back("FRI-SUN");   // true
        ranges.emplace_back("0-7");       // false
        ranges.emplace_back("0-0");       // false
        ranges.emplace_back("0-6");       // false
        ranges.emplace_back("0-5-10");    // false
        ranges.emplace_back("6");         // false
        ranges.emplace_back("4");         // false

        for(int index = 0; index < ranges.size(); index++) {
            bool result = CronRanges::weekday_range_matched(ranges[index]);

            if(index > 3) {
                // test that should be false
                result = !result;
            }

            if(!result) {
                std::cout << "weekday range test " << (index + 1) << ": "
                          << "failed !" << std::endl;
                return false;
            }

            std::cout << "weekday range test " << (index + 1) << ": "
                      << "passed." << std::endl;
        }

        std::cout << "All weekday range tests passed.\n"
                  << std::endl;
        return true;
    }

    auto test_all() -> bool {
        bool result = true;
        result &= second_ranges_test();
        result &= hour_ranges_test();
        result &= day_ranges_test();
        result &= month_ranges_test();
        result &= weekday_ranges_test();
        return result;
    }

}    // namespace CronTest
