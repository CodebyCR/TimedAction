cmake_minimum_required(VERSION 3.24)
project("TimedAction")

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

message(NOTICE "${PROJECT_NAME}: ${PROJECT_VERSION}")


add_executable(${PROJECT_NAME} main.cpp
        TimedAction_Types/TimedAction.hpp Cron/Cron.hpp TimedAction_Types/I_TimedAction.hpp TimedAction_Types/Light_TimedAction.hpp)
