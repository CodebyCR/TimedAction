//
// Created by Christoph Rohde on 21.12.22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

namespace StringUtils {

    /// Remove all chars out of the given string where equals the given char.<br/>
    /// Optional with case sensitive comparison, default is case insensitive.
    [[nodiscard]]
    static auto erase(std::string_view str, char character, bool case_sensitive=false) -> std::string {
        std::string result;
        case_sensitive
        ? std::ranges::copy_if(str, std::back_inserter(result), [&](char c) { return std::tolower(c) != std::tolower(character); })
        : std::ranges::copy_if(str, std::back_inserter(result), [&](char c) { return c != character; });

        return result;
    }

    /// Remove the first char that equal to the given char out of the given string.
    [[nodiscard]]
    static auto erase_first_of(std::string& str, char character) -> std::string {
        if(auto pos = str.find_first_of(character); pos != std::string::npos) {
            str.erase(pos, 1);
        }
        return str;
    }

    /// Remove the last char that equal to the given char out of the given string.
    [[nodiscard]]
    static auto erase_last_of(std::string& str, char character) -> std::string {
        if(auto pos = str.find_last_of(character); pos != std::string::npos) {
            str.erase(pos, 1);
        }
        return str;
    }

    /// Get a substring from the given string,
    /// starting at the given char(included)
    /// and ending at the given char(included).
    [[nodiscard]]
    static auto get_substring(std::string_view str, const char start, const char end) -> std::string {
        auto startPos = str.find_first_of(start);
        auto endPos = str.find_last_of(end);
        return str.substr(startPos, endPos - startPos + 1).data();
    }

    static auto split_by(std::string_view str, const char delimiter) -> std::vector<std::string> {
        auto result = std::vector<std::string>();
        auto start = 0;
        auto end = str.find_first_of(delimiter);

        while(end <= std::string::npos) {
            if(std::string_view current_sub_string = str.substr(start, end - start); !current_sub_string.empty()) {
                result.emplace_back(current_sub_string);
            }

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
    auto trim(std::string_view input) -> std::string_view {
//        auto trimChar = [](char c) { return std::isspace(c); };
//        auto start = std::ranges::find_if_not(input, trimChar);
//        auto end = std::ranges::find_if_not(input | std::views::reverse, trimChar).base();
        auto start = input.find_first_not_of(" \t\n\r\f\v");
        auto end = input.find_last_not_of(" \t\n\r\f\v");

        if(start == std::string::npos) {
            return "";
        }

        return input.substr(start, end - start + 1);
    }

    /**
     * returns a string as vector of strings, split by the delimiter characters
     * @param str
     * @param delimiters
     * @return
     */
    [[maybe_unused]]
    static auto split_by_any_of(std::string& str, std::string_view delimiters) -> std::vector<std::string> {
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
    [[maybe_unused]] [[nodiscard]]
    static auto find_leading_whitespace(std::string_view str, const char character) -> std::size_t {
        if(auto pos = str.find_first_of(character); pos != std::string::npos) {
            return str.find_first_not_of(" \t\n\r\f\v", pos);
        }
        return std::string::npos;
    }

    /// Find the given character in the given string
    /// and return the position of trailing whitespace, the position of the character if no trailing whitespace was found.
    /// or std::string::npos if the character was not found.
    [[maybe_unused]] [[nodiscard]]
    static auto find_trailing_whitespace(std::string_view str, const char character) -> std::size_t {
        if(auto pos = str.find_last_of(character); pos != std::string::npos) {
            return str.find_last_not_of(" \t\n\r\f\v", pos);
        }
        return std::string::npos;
    }

    /// Replace all occurrences of the given sequence in the given string with the given replacement.
    /// @param str
    /// @param sequence
    /// @param replacement
    /// @return
    [[maybe_unused]] [[nodiscard]]
    static auto replace_sequence(std::string_view str, std::string_view sequence, std::string_view replacement) -> std::string {
        auto result = std::string(str);
        auto pos = result.find(sequence);
        while(pos != std::string::npos) {
            result.replace(pos, sequence.length(), replacement);
            pos = result.find(sequence, pos + replacement.length());
        }
        return result;
    }

    static auto is_number(std::string_view str) -> bool {
        return !str.empty() &&
               std::ranges::find_if_not(str, [](char c) { return std::isdigit(c); }) == str.end();
    }

    static auto to_upper(std::string& str) -> void {
        std::ranges::transform(str, str.begin(), ::toupper);
    }

    /// Helper function to count occurrences of a character in a string
    [[maybe_unused]] [[nodiscard]]
    static auto count_occurrences(std::string_view str, char character) -> std::size_t {
        std::size_t count = 0;
        for(const char c: str) {
            if(c == character) {
                count++;
            }
        }
        return count;
//        return std::ranges::count(str, character);
        // ? compiler support for std::ranges::count
        // apple clang | yes
    }

    /// Splits the given string by the given delimiter, but only if the count of the mask is even.
    [[nodiscard]]
    static auto save_split(std::string_view str, char delimiter, char mask) -> std::vector<std::string> {
        std::vector<std::string> result;
        std::string currentString;
        std::size_t count = 0;

        for (char c : str) {
            if (c == mask) {
                count++;
            }

            if (c == delimiter && count % 2 == 0) {
                result.push_back(currentString);
                currentString.clear();
            } else {
                currentString += c;
            }
        }

        if (!currentString.empty()) {
            result.push_back(currentString);
        }

        return result;
    }


    /// For example: start_mask = '{', end_mask = '}', delimiter = ','.
    /// Splits the given string by the given delimiter, but ignored the masked part.
    /// NOTE: Can't handel equal start and end mask. Use 'save_split' if this is the case.
    [[nodiscard]]
    static auto solid_split(std::string_view input,
                            const char delimiter,
                            const char start_mask,
                            const char end_mask) -> std::vector<std::string> {
        std::vector<std::string> result;
        std::string currentString;
        bool closed_mask = true;

//        std::ranges::for_each(input, [&](char c) {
        std::ranges::for_each(input, [&](char c) {
            if(c == start_mask) { closed_mask = false; }
            else if(c == end_mask) { closed_mask = true; }

            if(c == delimiter && closed_mask) {
                result.push_back(currentString);
                currentString.clear();
            }
            else {
                currentString += c;
            }
        });

        if(!currentString.empty()) {
            result.push_back(currentString);
        }

        return result;
    }

    /// Splits the given string by the given delimiter.<br/>
    /// This function handels the processing for the given mask.
    [[maybe_unused]] [[nodiscard]]
    static auto ignore_mask_split(std::string_view str,
                                  const char delimiter,
                                  const char start_mask,
                                  const char end_mask ) -> std::vector<std::string> {
        return start_mask == end_mask
                ? save_split(str, delimiter, start_mask)
                : solid_split(str, delimiter, start_mask, end_mask);
    }

}    // namespace StringUtils
