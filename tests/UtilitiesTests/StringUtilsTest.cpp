//
// Created by Christoph Rohde on 19.08.23.
//

#include <iostream>

#include <gtest/gtest.h>
#include "../../TimedActionLib/Utilities/StringUtils.hpp"

class StringUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::cout << "StringUtils SetUp" << std::endl;
    }

    void TearDown() override {
        std::cout << "StringUtils TearDown" << std::endl;
    }


public:

};

TEST(StringUtilsTest, split_by) {
    const auto values = StringUtils::split_by("Hello World", ' ');
    ASSERT_EQ(values.size(), 2);
    ASSERT_EQ(values[0], "Hello");
    ASSERT_EQ(values[1], "World");
}

TEST(StringUtilsTest, solid_split) {
    std::string demoString = "Hello World: { Dieser Text wird nicht gesplittet.} ";
    const auto values = StringUtils::solid_split(demoString, ' ', '{', '}');

    ASSERT_EQ(values.size(), 3) << "The size of the vector should be equal to 3.";
    ASSERT_EQ(values[0], "Hello");
    ASSERT_EQ(values[1], "World:");
    ASSERT_EQ(values[2], "{ Dieser Text wird nicht gesplittet.}");
}

TEST(StringUtilsTest, save_split) {
    const std::string demoString = "This is a test: 'This text will not be split.' \"This Text will be split.\"";
    const auto values = StringUtils::save_split(demoString, ' ', '\'');

    ASSERT_EQ(values.size(), 10) << "The size of the vector should be equal to 3.";
    ASSERT_EQ(values[0], "This");
    ASSERT_EQ(values[1], "is");
    ASSERT_EQ(values[2], "a");
    ASSERT_EQ(values[3], "test:");
    ASSERT_EQ(values[4], "'This text will not be split.'");
    ASSERT_EQ(values[5], "\"This");
    ASSERT_EQ(values[6], "Text");
    ASSERT_EQ(values[7], "will");
    ASSERT_EQ(values[8], "be");
    ASSERT_EQ(values[9], "split.\"");
}

TEST(StringUtilsTest, split_by_multiple_delimiters) {
    const auto values = StringUtils::split_by("Split::By::Multiple::Colons", ':');
    EXPECT_EQ(values.size(), 4);
    EXPECT_EQ(values[0], "Split");
    EXPECT_EQ(values[1], "By");
    EXPECT_EQ(values[2], "Multiple");
    EXPECT_EQ(values[3], "Colons");
}

TEST(StringUtilsTest, trim) {
    const std::string demoString = "   This is a test.   ";
    const auto result = StringUtils::trim(demoString);
    EXPECT_EQ(result, "This is a test.");

    const std::string demoString2 = "   This is a test.";
    EXPECT_EQ(StringUtils::trim(demoString2), "This is a test.");

    const std::string demoString3 = "This is a test.    ";
    EXPECT_EQ(StringUtils::trim(demoString3), "This is a test.");
}