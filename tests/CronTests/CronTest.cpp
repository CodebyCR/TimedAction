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
    }

    void TearDown() override {
        std::cout << "CronTest TearDown" << std::endl;
    }


public:
    static auto getOutOfDateCron() -> Cron{
        return Cron({
                .second = "0",
                .minute = "30",
                .hour = "*/2",
                .dayOfMonth = "15",
                .month = "*/2",
                .weekday = "*",
                .year = "2023-2024"});
    }
};

TEST_F(CronTest, OPERATOR_TO_STRING) {
    Cron cron = CronTest::getOutOfDateCron();
    std::stringstream stream;
    stream << cron;
    std::string result = stream.str();

    EXPECT_EQ(result, "No execution times contained. -> out-of-date expression");
}

