## UtilitiesTests Tests
#set(UTILITIES_TESTS_SOURCES
#        "StringUtilsTest.cpp")
#
#add_library(${UTILITIES_TESTS} INTERFACE ${UTILITIES_TESTS_SOURCES} )
#
#
#target_include_directories( ${UTILITIES_TESTS}
#        INTERFACE
#        ${CMAKE_CURRENT_SOURCE_DIR} )

# Create an executable for your tests
add_executable(UtilitiesTest "StringUtilsTest.cpp")

# Link your library and Google Test
target_link_libraries(UtilitiesTest PRIVATE TimedActionLib gtest gtest_main)

# Add a test
add_test(NAME UtilitiesTest COMMAND UtilitiesTest)