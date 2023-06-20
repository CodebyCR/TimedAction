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
#include <filesystem>


/// .clangtaidy ->


class ConfigJSON: public std::map<std::string, std::string>{
public:

    ConfigJSON(const std::filesystem::path& path) {
        this->path = path;
        this->read();
    }

    auto to_string() -> std::string {
        std::string json = "{\n";
        for(auto& [key, value] : *this) {
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

    // update on set


    // user defined literal _json
//    auto operator""_json(const char* jsonString) -> void {
//        this->parse_json_string(jsonString);
//    }
//
//    auto operator""_json(const std::string& jsonString) -> void {
//        this->parse_json_string(jsonString);
//    }





private:
    std::filesystem::path path;

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



    auto parse_json_string(std::string& jsonString) -> void {
        // Remove whitespaces and newlines from the JSON string
        jsonString.erase(std::remove_if(jsonString.begin(), jsonString.end(),
                                        [](unsigned char c) { return std::isspace(c); }),
                         jsonString.end());


        // get pair
        for(int index = 0; index < jsonString.length(); index++) {
            if(jsonString[index] == '{') {
                jsonString.erase(index, 1);
            }
            if(jsonString[index] == '}') {
                jsonString.erase(index, 1);
            }
        }

        jsonString = StringUtils::eraseChar(jsonString, '"');

        std::vector<std::string> pairs = StringUtils::split_by(jsonString, ',');
        for(const auto& pair: pairs) {
            std::vector<std::string> keyValue = StringUtils::split_by(pair, ':');
            const auto currentPair = std::pair<std::string, std::string>(keyValue.at(0), keyValue.at(1));

            this->insert(currentPair);
        }

    }

    auto read() -> void {
        std::string json_input = this->readJsonFile();
        return this->parse_json_string(json_input);
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
