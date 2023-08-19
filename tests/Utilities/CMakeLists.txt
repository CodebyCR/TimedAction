# Utilities Tests
set(UTILITIES_TESTS_SOURCES
        "StringUtilsTest.cpp")

add_library(${UTILITIES_TESTS} INTERFACE ${UTILITIES_TESTS_SOURCES} )


target_include_directories( ${UTILITIES_TESTS}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )