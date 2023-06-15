//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace StringUtils { //TODO: full static ?

    /// Remove all chars out of the given string where equals the given char.
    [[nodiscard]]
    static auto eraseChar(std::string const& str, char character) -> std::string {
        std::string result;
        for (char c : str) {
            if (c != character) {
                result += c;
            }
        }
        return result;
    }

    static auto eraseChars(std::string const& str, char** chars) -> std::string {
        // TODO: implement
    }

    auto split_by(std::string_view const &str, char const &delimiter) -> std::vector<std::string> {
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


    /**
     * returns a string as vector of strings, split by the delimiter characters
     * @param str
     * @param delimiters
     * @return
     */
    auto split_by_any_of(std::string &str, std::string_view const &delimiters) -> std::vector<std::string> {
        std::vector<std::string> result;

        if (str.empty()) {
            return result;
        }

        for (auto delimiter: delimiters) {
            auto split = split_by(str, delimiter);

            auto currentDelimiter = std::string(1, delimiter);
            result.push_back(split.at(0));
            result.push_back(currentDelimiter);

            if (split.size() > 1) {
                str = split.at(1);
            }
        }

        result.push_back(str);


        return result;
    }

    auto is_number(std::string_view const &str) -> bool {
        return !str.empty() &&
               std::find_if(str.begin(), str.end(), [](char c) { return !std::isdigit(c); }) == str.end();
    }

    auto count_occurrences(std::string_view const &str, char const &character) -> uint8_t {
        return std::count(str.begin(), str.end(), character);
    }


    auto to_upper(std::string &str) -> void {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

}

