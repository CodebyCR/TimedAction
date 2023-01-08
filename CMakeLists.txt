cmake_minimum_required(VERSION 3.24)
project("TimedAction")

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

message(NOTICE "${PROJECT_NAME}: ${PROJECT_VERSION}")

set(LIBRARY_CRON "Cron")
add_subdirectory(${LIBRARY_CRON})
if (LIBRARY_CRON)
    message(NOTICE " - [1/2] ${LIBRARY_CRON} found")
else()
    message(FATAL_ERROR " - [1/2] ${LIBRARY_CRON} not found")
endif ()

set(LIBRARY_TIMED_ACTION "TimedAction_Types")
add_subdirectory(${LIBRARY_TIMED_ACTION})
if (LIBRARY_TIMED_ACTION)
    message(NOTICE " - [2/2] ${LIBRARY_TIMED_ACTION} found")
else()
    message(FATAL_ERROR " - [2/2] ${LIBRARY_TIMED_ACTION} not found")
endif ()



add_executable(${PROJECT_NAME} main.cpp Scheduler.hpp StringUtils.hpp )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${LIBRARY_CRON}
        ${LIBRARY_TIMED_ACTION})
