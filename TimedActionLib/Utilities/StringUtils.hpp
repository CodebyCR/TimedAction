//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace StringUtils {

    /// Remove all chars out of the given string where equals the given char.
    [[nodiscard]]
    static auto eraseChar(std::string const& str, char character) -> std::string {
        std::string result;
        for(char c: str) {
            if(c != character) {
                result += c;
            }
        }
        return result;
    }

    /// Remove the first char that equal to the given char out of the given string.
    [[nodiscard]]
    static auto erase_first_of(std::string& str, char character) -> std::string {
        auto pos = str.find_first_of(character);
        if(pos != std::string::npos) {
            str.erase(pos, 1);
        }
        return str;
    }

    /// Remove the last char that equal to the given char out of the given string.
    [[nodiscard]]
    static auto erase_last_of(std::string& str, char character) -> std::string {
        auto pos = str.find_last_of(character);
        if(pos != std::string::npos) {
            str.erase(pos, 1);
        }
        return str;
    }

    /// Get a substring from the given string,
    /// starting at the given char(included)
    /// and ending at the given char(included).
    [[nodiscard]]
    static auto get_substring(std::string const& str, char start, char end) -> std::string {
        auto startPos = str.find_first_of(start);
        auto endPos = str.find_last_of(end);
        return str.substr(startPos, endPos - startPos + 1);
    }

    static auto split_by(std::string_view const& str, char const& delimiter) -> std::vector<std::string> {
        auto result = std::vector<std::string>();
        auto start = 0;
        auto end = str.find_first_of(delimiter);

        while(end <= std::string::npos) {
            result.emplace_back(str.substr(start, end - start));

            if(end == std::string::npos) {
                break;
            }

            start = end + 1;
            end = str.find_first_of(delimiter, start);
        }

        return result;
    }


    /// Erase leading and trailing whitespaces & line breaks from the given string.
    [[nodiscard]]
    static auto trim(std::string const& str) -> std::string {
        auto start = str.find_first_not_of(" \t\n\r\f\v");
        auto end = str.find_last_not_of(" \t\n\r\f\v");

        if(start == std::string::npos) {
            return "";
        }

        return str.substr(start, end - start + 1);
    }


    /**
     * returns a string as vector of strings, split by the delimiter characters
     * @param str
     * @param delimiters
     * @return
     */
    static auto split_by_any_of(std::string& str, std::string_view const& delimiters) -> std::vector<std::string> {
        std::vector<std::string> result;

        if(str.empty()) {
            return result;
        }

        for(auto delimiter: delimiters) {
            auto split = split_by(str, delimiter);
            auto currentDelimiter = std::string(1, delimiter);
            result.push_back(split.at(0));
            result.push_back(currentDelimiter);

            if(split.size() > 1) {
                str = split.at(1);
            }
        }
        result.push_back(str);

        return result;
    }

    /// Find the given character in the given string
    /// and return the position of leading whitespace, the position of the character if no leading whitespace was found.
    /// or std::string::npos if the character was not found.
    [[nodiscard]]
    static auto find_leading_whitespace(std::string_view str, const char character) -> std::size_t {
        auto pos = str.find_first_of(character);
        if(pos != std::string::npos) {
            return str.find_first_not_of(" \t\n\r\f\v", pos);
        }
        return std::string::npos;
    }

    /// Find the given character in the given string
    /// and return the position of trailing whitespace, the position of the character if no trailing whitespace was found.
    /// or std::string::npos if the character was not found.
    [[nodiscard]]
    static auto find_trailing_whitespace(std::string_view str, const char character) -> std::size_t {
        auto pos = str.find_last_of(character);
        if(pos != std::string::npos) {
            return str.find_last_not_of(" \t\n\r\f\v", pos);
        }
        return std::string::npos;
    }

    /// Replace all occurrences of the given sequence in the given string with the given replacement.
    /// @param str
    /// @param sequence
    /// @param replacement
    /// @return
    [[nodiscard]]
    static auto replace_sequence(std::string_view str, std::string_view const& sequence, std::string_view const& replacement) -> std::string {
        auto result = std::string(str);
        auto pos = result.find(sequence);
        while(pos != std::string::npos) {
            result.replace(pos, sequence.length(), replacement);
            pos = result.find(sequence, pos + replacement.length());
        }
        return result;
    }

    static auto is_number(std::string_view const& str) -> bool {
        return !str.empty() &&
               std::find_if(str.begin(), str.end(), [](char c) { return !std::isdigit(c); }) == str.end();
    }

    static auto to_upper(std::string& str) -> void {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    /// Helper function to count occurrences of a character in a string
    [[nodiscard]] [[deprecated("Use std::ranges::count instead")]]
    static auto count_occurrences(std::string_view str, char character) -> std::uint32_t {
        return std::ranges::count(str, character);
    }

    /// Splits the given string by the given delimiter, but only if the count of the mask is even.
    [[nodiscard]] [[deprecated("Will be replaced by a string_view version")]]
    static auto save_split(std::string& str, char delimiter, char mask) -> std::vector<std::string> {
        std::vector<std::string> result;
        std::string currentString;
        std::uint32_t count = 0;

        for(char c: str) {
            if(c == mask) {
                count++;
            }

            if(c == delimiter && count % 2 == 0) {
                result.push_back(currentString);
                currentString.clear();
            }
            else {
                currentString += c;
            }
        }

        if(!currentString.empty()) {
            result.push_back(currentString);
        }

        return result;
    }

    /// For example: start_mask = '{', end_mask = '}', delimiter = ','.
    /// Splits the given string by the given delimiter, but don't in the mask.
    [[nodiscard]] [[deprecated("Will be replaced by a string_view version")]]
    static auto solid_split(const std::string& input,
                            const char delimiter,
                            const char start_mask,
                            const char end_mask) -> std::vector<std::string> {
        std::vector<std::string> result;
        std::string currentString;
        std::uint32_t count1 = 0;
        std::uint32_t count2 = 0;

        for(char c: input) {
            if(c == start_mask) {
                count1++;
            }
            else if(c == end_mask) {
                count2++;
            }

            if(c == delimiter && count1 == count2) {
                result.push_back(currentString);
                currentString.clear();
            }
            else {
                currentString += c;
            }
        }

        if(!currentString.empty()) {
            result.push_back(currentString);
        }

        return result;
    }
}    // namespace StringUtils
