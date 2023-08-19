//
// Created by Christoph Rohde on 17.08.23.
//
#include <iostream>

#include <gtest/gtest.h>
#include "Cron.hpp"

class CronTest : public ::testing::Test {
protected:

    void SetUp() override {
        std::cout << "CronTest SetUp" << std::endl;
        auto cron = Cron({
                .second = "0",
                .minute = "30",
                .hour = "*/2",
                .dayOfMonth = "15",
                .month = "*/2",
                .weekday = "*",
                .year = "2023-2024"});

        std::cout << cron << std::endl;

    }

    void TearDown() override {
        std::cout << "CronTest TearDown" << std::endl;
    }


};

TEST_F(CronTest, test) {
    std::cout << "CronTest test" << std::endl;
    EXPECT_EQ(1, 1);
}

