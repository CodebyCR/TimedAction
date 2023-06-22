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
#include <variant>


/// This namespace ist for the parsing of JSON config files.

namespace JSONParser {

    static auto readJsonFile(std::filesystem::path const& filename) -> std::string {
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





//    [[deprecated("use ConfigJSON instead")]]
//    static auto json_to_string_map(std::filesystem::path const& filename) -> std::map<std::string, std::string> {
//        std::string json_input = JSONParser::readJsonFile(filename);
//        return JSONParser::parse_string_map(json_input);
//    }

    static auto map_to_json_string(std::map<std::string, std::string> const& stringMap) -> std::string {
        /// * refactor with stringstream
        //        auto json = std::stringstream();
        //        json << "{" << '\n';
        //        for (auto const& [key, value] : stringMap) {
        //            json << "\"" << key << "\":\"" << value << "\",";
        //        }

        std::string json = "{\n";
        for(auto const& [key, value]: stringMap) {
            json.append("\"")
                    .append(key)
                    .append("\":\"")
                    .append(value)
                    .append("\",\n");
        }
        json.pop_back();
        json.pop_back();
        json.append("\n}");

        return json;
    }

    [[deprecated("use ConfigJSON instead")]]
    static auto string_map_to_json(std::map<std::string, std::string>& stringMap) -> std::filesystem::path {
        auto settings_json_path = std::filesystem::path("/Users/christoph_rohde/Example/Scheduler.json");
        // std::filesystem::path(stringMap.at("Settings_JSON")); /// Update settings.json Path
        const auto json_string = map_to_json_string(stringMap);

        if(!exists(settings_json_path) || !is_regular_file(settings_json_path)) {
            std::cout << "File does not exist" << std::endl;
        }

        stringMap.at("test2") = "new Value";

        std::cout << json_string << std::endl;

        if(std::ofstream fileStream(settings_json_path); fileStream.is_open()) {
            fileStream << json_string;
            fileStream.close();
            return settings_json_path;
        }

        std::cerr << "File can't be opened. Path: " << settings_json_path << std::endl;
        return "";
    }


}    // namespace JSONParser
