//
// Created by Christoph Rohde on 24.04.23.
//

#pragma once

#include "Message.hpp"
#include <chrono>
#include <iostream>
#include <sstream>


namespace Log {
    constexpr std::string_view SUCCESS = "\x1b[32mSUCCESS\x1b[0m";
    constexpr std::string_view FAILURE = "\x1b[31mFAILURE\x1b[0m";
    constexpr std::string_view WARNING = "\x1b[33mWARNING\x1b[0m";
    constexpr std::string_view INFO = "\x1b[34mINFO\x1b[0m";
}    // namespace Log

class JobLog : public Message {
private:

    std::vector<std::pair<std::string_view, std::string_view>> log;

    static auto getTime(std::string_view const& logType) -> std::string {
        const auto now = std::time(nullptr);
        auto ss = std::stringstream();

        ss << "[" << std::asctime(std::localtime(&now)) << "::" << logType << "]";
        return ss.str();
    }

public:
    JobLog(std::string_view name, std::string_view executionTime) : Message(name, executionTime) {}

    ~JobLog() override = default;

    // copy constructor
    JobLog(std::string_view name, std::string_view executionTime, JobLog const& other) : Message(name, executionTime), log(other.log) {}

    // copy assignment
    auto operator=(JobLog const& other) -> JobLog& {
        if (this != &other) {
            Message::operator=(other);
            log = other.log;
        }
        return *this;
    }

    // move constructor
    JobLog(JobLog&& other) noexcept : Message(other), log(std::move(other.log)) {}

    // move assignment
    auto operator=(JobLog&& other) noexcept -> JobLog& {
        if (this != &other) {
            Message::operator=(other);
            log = std::move(other.log);
        }
        return *this;
    }

    auto INFO(std::string_view message) -> void {
        log.emplace_back(getTime(Log::INFO), message);
    }

    auto SUCCESS(std::string_view message) -> void {
        log.emplace_back(getTime(Log::SUCCESS), message);
    }

    auto FAILURE(std::string_view message) -> void {
        log.emplace_back(getTime(Log::FAILURE), message);
    }

    auto WARNING(std::string_view message) -> void {
        log.emplace_back(getTime(Log::WARNING), message);
    }

    auto print() -> void {
        std::ranges::for_each(log, [](auto& logEntry) {
            std::cout << logEntry.first << " " << logEntry.second << std::endl;
        });
    }
};
