cmake_minimum_required(VERSION 3.20..3.24)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 4)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project(
        "TimedAction"
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "An easy, state-of-the-art timing Framework."
        LANGUAGES CXX)

message(NOTICE "${PROJECT_NAME}: ${PROJECT_VERSION}")

set(TIMED_ACTION_LIBS "TimedActionLib")
add_subdirectory(${TIMED_ACTION_LIBS})

#include("cmake/UpdateSubmodules.cmake")

# Enable testing
enable_testing()
include(CTest)

set(LIBRARY_TESTS "tests")
add_subdirectory(${LIBRARY_TESTS})


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${TIMED_ACTION_LIBS})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${TIMED_ACTION_LIBS}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
