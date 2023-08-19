//
// Created by Christoph Rohde on 19.08.23.
//

#include <iostream>

#include <gtest/gtest.h>
#include "StringUtils.hpp"

TEST(StringUtils, split) {
    const auto values = StringUtils::split("Hello World", ' ');
    ASSERT_EQ(values.size(), 2);
    ASSERT_EQ(values[0], "Hello");
    ASSERT_EQ(values[1], "World");
}


