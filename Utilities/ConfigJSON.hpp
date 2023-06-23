//
// Created by Christoph Rohde on 31.05.23.
//

#pragma once

#include "StringUtils.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>

/** BUG: map should be a linked map*/
class ConfigJSON {
public:
    explicit ConfigJSON(const std::filesystem::path &path) {
        this->path = path;
        this->read();
    }

    explicit ConfigJSON(std::string &json) {
        this->get_structure_out_of_string(json);
        /// ? Path
    }

    auto to_string() -> std::string {
        std::string json = "{\n";
        int indentLevel = 1;
        std::string indent = "    ";

        for (auto const& [key, value] : this->self) {
            json += indent;
            json += "\"" + key + "\": ";

            if (std::holds_alternative<std::string>(value)) {
                json += "\"" + std::get<std::string>(value) + "\",\n";
            } else if (std::holds_alternative<std::map<std::string, std::string>>(value)) {
                json += "{\n";
                for (auto const& [key2, value2] : std::get<std::map<std::string, std::string>>(value)) {
                    json += indent + indent;
                    json += "\"" + key2 + "\": \"" + value2 + "\",\n";
                }
                json.erase(json.length() - 2); // Entferne das Komma am Ende des inneren Objekts
                json += indent + "},\n";
            }
        }

        json.erase(json.length() - 2); // Entferne das Komma am Ende des äußeren Objekts
        json += "\n" + indent.substr(0, indent.length() - 1) + "}";

        return json;
    }


    auto get_optional_map(std::string const &key) -> std::optional<std::map<std::string, std::string>> {
        if (this->self.contains(key)) {
            return std::get<std::map<std::string, std::string>>(this->self.at(key));
        }
        return std::nullopt;
    }

    auto insert_into_sub_map(std::string const &key, std::string const &sub_key, std::string const &value) -> void {
        if (this->self.contains(key)) {
            std::get<std::map<std::string, std::string>>(this->self.at(key)).insert(std::pair(sub_key, value));
        } else {
            std::map<std::string, std::string> sub_map;
            sub_map.insert(std::pair(sub_key, value));
            this->self.insert(std::pair(key, sub_map));
        }
        if (this->path != "") { this->write(); }
    }


private:
    std::filesystem::path path;
    std::map<std::string, std::variant<std::string, std::map<std::string, std::string>>> self;

    auto readJsonFile() -> std::string {
        std::ifstream file(this->path);
        if (!file.is_open()) {
            std::cerr << "Fehler beim Öffnen der Datei: " << this->path << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return buffer.str();
    }

    /// parse a key value pair from a string
    static auto split_key_value_pair(std::string &pair) -> std::pair<std::string, std::string> {
        std::vector<std::string> keyValue = StringUtils::save_split(pair, ':', '"');
        const auto raw_key = StringUtils::trim(keyValue[0]);
        const auto key = StringUtils::eraseChar(raw_key, '"');
        const auto raw_value = StringUtils::trim(keyValue[1]);
        const auto value = StringUtils::eraseChar(raw_value, '"');

        return std::pair{key, value};
    }


    /// Parse a std::map<std::string, std::string> from a string.
    static auto parse_string_map(std::string &sub_map) -> std::map<std::string, std::string> {
        std::map<std::string, std::string> translationMap;

        // get pair
        auto result = StringUtils::erase_first_of(sub_map, '{');
        result = StringUtils::erase_last_of(result, '}');

        /// Split in key value pair strings
        std::vector<std::string> keyValue = StringUtils::save_split(result, ',', '"');
        for (auto &pair: keyValue) {
            auto [key, value] = split_key_value_pair(pair);
//            std::cout << "Key: " << key << std::endl;
//            std::cout << "Value: " << value << '\n' << std::endl;
            translationMap.insert(std::pair{key, value});
        }

        return translationMap;
    }

    /// Insert the key value pairs into the Object
    auto insert_key_variant_pairs(std::vector<std::string> const &key_variant_pairs) -> void {
        for (auto &pair: key_variant_pairs) {
            auto current_map_pair = StringUtils::solid_split(pair, ':', '{', '}');
            std::string key = StringUtils::eraseChar(
                    StringUtils::get_substring(current_map_pair.at(0), '"', '"'),
                    '"');

            std::string raw_value = current_map_pair.at(1);
            auto value = std::variant<std::string, std::map<std::string, std::string>>();

            if (bool is_map = raw_value.find('{') != std::string::npos; is_map) {
                value = ConfigJSON::parse_string_map(raw_value);
            } else {
                value = StringUtils::eraseChar(raw_value, '"');
            }

            auto current_pair = std::pair<std::string, std::variant<std::string, std::map<std::string, std::string>>>(
                    key, value);
            this->self.insert(current_pair);
        }
    }

    auto get_structure_out_of_string(std::string &jsonString) -> void {
        /// Throw error if something went wrong.
        if (jsonString.empty()) {
            std::cerr << "JSON-String is empty!" << std::endl;
            return;
        }

        /// Step 1: Remove first and last curly braces
        auto result = StringUtils::erase_first_of(jsonString, '{');
        result = StringUtils::erase_last_of(result, '}');

        /// Split by ',' but ignore '{' - '}' areas
        /// After that you got the key - sub map pairs
        auto key_variant_pairs = StringUtils::solid_split(result, ',', '{', '}');

        this->insert_key_variant_pairs(key_variant_pairs);
    }

    /// Read the file and parse the content into the object.
    auto read() -> void {
        std::string json_input = this->readJsonFile();
        this->get_structure_out_of_string(json_input);
    }

    /// Write the object into the file.
    auto write() -> void {
        std::ofstream fileStream(this->path);
        if (!fileStream.is_open()) {
            std::cerr << "File can't be opened. Path: " << this->path << std::endl;
            return;
        }

        fileStream << this->to_string();
        fileStream.close();
    }
};
