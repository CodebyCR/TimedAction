cmake_minimum_required(VERSION 3.24)
project("TimedAction")

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

message(NOTICE "${PROJECT_NAME}: ${PROJECT_VERSION}")


set(LIBRARY_TIMED_ACTION "TimedAction_Types")
add_subdirectory(${LIBRARY_TIMED_ACTION})


add_executable(${PROJECT_NAME} main.cpp Cron/CronRegex.hpp Scheduler.hpp Cron/UTC_Parser.hpp StringUtils.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${LIBRARY_TIMED_ACTION}
        Cron)
