## CronTests Tests
#set(CRON_TESTS_SOURCES
#        "CronTest.cpp")
#
#add_library(${CRON_TESTS} INTERFACE ${CRON_TESTS_SOURCES} )
#
#
#target_include_directories( ${CRON_TESTS}
#        INTERFACE
#        ${CMAKE_CURRENT_SOURCE_DIR} )
#
## CMakeLists.txt in the 'CronTests' directory

# Create an executable for your tests
add_executable(CronTests CronTest.cpp)

# Link your library and Google Test
target_link_libraries(CronTests PRIVATE TimedActionLib gtest gtest_main)

# Add a test
add_test(NAME CronTests COMMAND CronTests)
