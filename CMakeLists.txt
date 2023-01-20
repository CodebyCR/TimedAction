cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project("TimedAction" VERSION ${PROJECT_VERSION} LANGUAGES CXX)

message(NOTICE "${PROJECT_NAME}: ${PROJECT_VERSION}")

set(LIBRARY_CRON "Cron")
add_subdirectory(${LIBRARY_CRON})
if (LIBRARY_CRON)
    message(NOTICE " - [1/2] ${LIBRARY_CRON} found")
else()
    message(FATAL_ERROR " - [1/2] ${LIBRARY_CRON} not found")
endif()

set(LIBRARY_TIMED_ACTION "TimedAction_Types")
add_subdirectory(${LIBRARY_TIMED_ACTION})
if (LIBRARY_TIMED_ACTION)
    message(NOTICE " - [2/2] ${LIBRARY_TIMED_ACTION} found")
else()
    message(FATAL_ERROR " - [2/2] ${LIBRARY_TIMED_ACTION} not found")
endif()




# Create new library variable
set(TIMED_ACTION_LIBS "TimedAction_Libs")


# Bind libraries to one Library
add_library(${TIMED_ACTION_LIBS} SHARED
        ${LIBRARY_CRON}
        ${LIBRARY_TIMED_ACTION}
        "Scheduler.hpp"
        "StringUtils.hpp")


add_executable(${PROJECT_NAME} main.cpp )

# Link the libraries
set_target_properties(${TIMED_ACTION_LIBS} PROPERTIES LINKER_LANGUAGE CXX)
include_directories(${TIMED_ACTION_LIBS})



# Create a library for find_package
install(TARGETS ${PROJECT_NAME}
        EXPORT ${TIMED_ACTION_LIBS}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Create a config file for find_package
install(TARGETS ${TIMED_ACTION_LIBS}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)