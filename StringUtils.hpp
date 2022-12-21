//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace StringUtils{

    auto split_by(std::string_view const& str, char const& delimiter) -> std::vector<std::string> {
        auto result = std::vector<std::string>();
        auto start = 0;
        auto end = str.find_first_of(delimiter);

        while (end <= std::string::npos) {
            result.emplace_back(str.substr(start, end - start));

            if (end == std::string::npos) {
                break;
            }

            start = end + 1;
            end = str.find_first_of(delimiter, start);
        }

        return result;
    }

}

