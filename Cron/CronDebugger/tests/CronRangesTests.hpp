//
// Created by Christoph Rohde on 09.06.23.
//

#pragma once
#include <iostream>
#include "../CronRange.hpp"


namespace CronTest{

    auto second_ranges_test() -> bool{

        auto ranges = std::vector<std::string>();
        ranges.push_back("0-59"); // true
        ranges.push_back("13-29"); // true
        ranges.push_back("5-19"); // true
        ranges.push_back("0-60"); // false
        ranges.push_back("0-0"); // false
        ranges.push_back("59-59"); // false
        ranges.push_back("0-5-10"); // false
        ranges.push_back("59"); // true
        ranges.push_back("12"); // true

        for(int index = 0; index < ranges.size(); index++){
            bool result = CronRange::second_range_matched(ranges[index]);

            if(index > 2){
                // test that should be false
                result = !result;
            }

            if(!result){
                std::cout << "second range test " << (index + 1) << ": " << "failed !" << std::endl;
                return false;
            }

            std::cout << "second range test " << (index + 1) << ": " << "passed." << std::endl;

        }

        std::cout << "All second range tests passed.\n" << std::endl;
        return true;
    }

    auto hour_ranges_test() -> bool{
        auto ranges = std::vector<std::string>();
        ranges.push_back("0-23");   // true
        ranges.push_back("13-23");  // true
        ranges.push_back("5-19");   // true
        ranges.push_back("0-24");   // false
        ranges.push_back("0-0");    // false
        ranges.push_back("23-23");  // false
        ranges.push_back("0-5-10"); // false
        ranges.push_back("23");     // false
        ranges.push_back("12");     // false

        for(int index = 0; index < ranges.size(); index++){
            bool result = CronRange::hour_range_matched(ranges[index]);

            if(index > 2){
                // test that should be false
                result = !result;
            }

            if(!result){
                std::cout << "hour range test " << (index + 1) << ": " << "failed !" << std::endl;
                return false;
            }

            std::cout << "hour range test " << (index + 1) << ": " << "passed." << std::endl;

        }

        std::cout << "All hour range tests passed.\n" << std::endl;
        return true;
    }

    auto day_ranges_test() -> bool{
        auto ranges = std::vector<std::string>();
        ranges.push_back("1-31");   // true
        ranges.push_back("13-31");  // true
        ranges.push_back("5-19");   // true
        ranges.push_back("0-32");   // false
        ranges.push_back("0-0");    // false
        ranges.push_back("0-31");   // false
        ranges.push_back("0-5-10"); // false
        ranges.push_back("31");     // false
        ranges.push_back("12");     // false

        for(int index = 0; index < ranges.size(); index++){
            bool result = CronRange::day_range_matched(ranges[index]);

            if(index > 2){
                // test that should be false
                result = !result;
            }

            if(!result){
                std::cout << "day range test " << (index + 1) << ": " << "failed !" << std::endl;
                return false;
            }

            std::cout << "day range test " << (index + 1) << ": " << "passed." << std::endl;

        }

        std::cout << "All day range tests passed.\n" << std::endl;
        return true;
    }

    auto test_all() -> bool{
        bool result = true;
        result &= second_ranges_test();
        result &= hour_ranges_test();
        result &= day_ranges_test();
        return result;
    }

}

