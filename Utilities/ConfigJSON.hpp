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
    explicit ConfigJSON(const std::filesystem::path& path) {
        this->path = path;
        this->read();
    }

    auto to_string() -> std::string {
        std::string json = "{\n";
        for(auto const& [key, value]: this->self) {
            if(std::holds_alternative<std::string>(value)) {
                json += "\"" + key + "\":\"" + std::get<std::string>(value) + "\",";
            }
            else if(std::holds_alternative<std::map<std::string, std::string>>(value)) {
                json += "\"" + key + "\":{";
                for(auto const& [key2, value2]: std::get<std::map<std::string, std::string>>(value)) {
                    json.append("\"" + key2 + "\":\"" + value2 + "\",");
                }
                json.pop_back();
                json += "},";
            }
        }
        json.pop_back();
        json += "\n}";

        return json;
    }

    auto get_optional_map(std::string const& key) -> std::optional<std::map<std::string, std::string>> {
        if(this->self.contains(key)) {
            return std::get<std::map<std::string, std::string>>(this->self.at(key));
        }
        return std::nullopt;
    }

    auto insert(std::string const& key, const std::string& value) -> void {
        this->self.insert(std::pair(key, value));
        this->write();
    }


    // update on set


    // user defined literal _json
    //    auto operator""_json(const char* jsonString) -> void {
    //        this->parse_string_map(jsonString);
    //    }
    //
    //    auto operator""_json(const std::string& jsonString) -> void {
    //        this->parse_string_map(jsonString);
    //    }


private:
    std::filesystem::path path;
    std::map<std::string, std::variant<std::string, std::map<std::string, std::string>>> self;

    auto readJsonFile() -> std::string {
        std::ifstream file(this->path);
        if(!file.is_open()) {
            std::cerr << "Fehler beim Öffnen der Datei: " << this->path << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return buffer.str();
    }


    static auto parse_string_map(std::string& jsonString) -> std::map<std::string, std::string> {
        std::map<std::string, std::string> translationMap;

        // get pair
        auto result = StringUtils::erase_first_of(jsonString, '{');
        result = StringUtils::erase_last_of(result, '}');



//        jsonString = StringUtils::eraseChar(jsonString, '"');

        std::vector<std::string> keyValue = StringUtils::solid_split(result, ':', '"', '"');

        for(auto& pair: keyValue) {
            std::vector<std::string> pairs2 = StringUtils::solid_split(pair, ':', '"', '"');
            for(auto& pair2: pairs2) {
                std::cout << "***" << pair << "***" << std::endl;
            }
        }
//        std::cout << "--------------------Test--------------------" << std::endl;
////        std::cout << jsonString << std::endl;
//                        for(auto& pair: keyValue) {
//                            std::cout << "***" << pair << "***" << std::endl;
//                        }
//        std::cout << "--------------------------------------------" << std::endl;

//        for(auto& pairs: keyValue) {
//            std::cout << "---" <<pairs << "---" << std::endl;
//            //            std::vector<std::string> pair = StringUtils::split_by(pairs, ',');
//            //            auto currentPair = std::pair<std::string, std::string>(keyValue.at(0), keyValue.at(1));
//            //
//            //            translationMap.insert(currentPair);
//        }

//        for(auto const& pair : translationMap) {
//            std::cout << "--------------------------------------------" << pair.first << " - " << pair.second << std::endl;
//        }

        return translationMap;
    }

    auto insert_key_variant_pairs(std::vector<std::string> const& key_variant_pairs) -> void {
        for(auto& pair: key_variant_pairs) {
            auto current_map_pair = StringUtils::solid_split(pair, ':', '{', '}');
            std::string key = StringUtils::eraseChar(
                    StringUtils::get_substring(current_map_pair.at(0), '"', '"'),
                    '"');

            for(auto& c: current_map_pair) {
                std::cout << c << std::endl;
            }
//--------------------------------------------

            std::string raw_value = current_map_pair.at(1);
            auto value = std::variant<std::string, std::map<std::string, std::string>>();

            if(bool is_map = raw_value.find('{') != std::string::npos; is_map) {
                auto map = ConfigJSON::parse_string_map(raw_value);

//
//                for(auto const& pair : map) {
//                    std::cout << "--------------------------------------------" << pair.first << " - " << pair.second << std::endl;
//                }

                value = map;
            }
            else {
                value = StringUtils::eraseChar(raw_value, '"');
            }

            auto current_pair = std::pair<std::string, std::variant<std::string, std::map<std::string, std::string>>>(
                    key, value);
            this->self.insert(current_pair);
        }
    }

    auto get_structure_out_of_string(std::string& jsonString) -> void {
        /// Throw error if something went wrong.
        if(jsonString.empty()) {
            std::cerr << "JSON-String is empty!" << std::endl;
            return;
        }

        /// Step 1: Remove first and last curly braces
        auto result = StringUtils::erase_first_of(jsonString, '{');
        result = StringUtils::erase_last_of(result, '}');

        /// Split by ',' but ignore '{' - '}' areas
        auto key_variant_pairs = StringUtils::solid_split(result, ',', '{', '}');

//        std::for_each(key_variant_pairs.begin(), key_variant_pairs.end(), [](std::string& str) {
//            std::cout << "--------------------------------------------" << str << std::endl;
//        });




        this->insert_key_variant_pairs(key_variant_pairs);
    }

    auto read() -> void {
        std::string json_input = this->readJsonFile();
        this->get_structure_out_of_string(json_input);
    }

    auto write() -> void {
        std::ofstream fileStream(this->path);
        if(!fileStream.is_open()) {
            std::cerr << "File can't be opened. Path: " << this->path << std::endl;
            return;
        }

        fileStream << this->to_string();
        fileStream.close();
    }
};
