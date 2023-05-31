//
// Created by Christoph Rohde on 31.05.23.
//

#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

#include "StringUtils.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>

namespace JSONParser {

    auto readJsonFile(const std::string& filename) -> std::string {
        std::ifstream file(filename);
        if(!file.is_open()) {
            std::cerr << "Fehler beim Öffnen der Datei: " << filename << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return buffer.str();
    }

    auto parse_json_string(std::string& jsonString) -> std::map<std::string, std::string> {
        // Remove whitespaces and newlines from the JSON string
        jsonString.erase(std::remove_if(jsonString.begin(), jsonString.end(),
                                        [](unsigned char c) { return std::isspace(c); }),
                         jsonString.end());

        //        std::cout << "After trim: " << jsonString << std::endl;

        std::map<std::string, std::string> translationMap;

        // get pair
        for(int index = 0; index < jsonString.length(); index++) {
            if(jsonString[index] == '{') {
                jsonString.erase(index, 1);
            }
            if(jsonString[index] == '}') {
                jsonString.erase(index, 1);
            }
        }

        //        std::cout << "After remove braces: " << jsonString << std::endl;

        // Split by ','
        std::vector<std::string> pairs = StringUtils::split_by(jsonString, ',');
        for(const auto& pair: pairs) {
            std::vector<std::string> keyValue = StringUtils::split_by(pair, ':');

            std::string_view key = keyValue.at(0);
            key.substr(1, keyValue.at(0).length() - 2);

            std::string_view value = keyValue.at(1);
            value.substr(1, keyValue.at(0).length());


            auto currentPair = std::pair<std::string, std::string>(key, value);
            std::cout << "Pair: " << currentPair.first << '-' << currentPair.second << std::endl;
            translationMap.insert(currentPair);
        }


        return translationMap;
    }

}    // namespace JSONParser
