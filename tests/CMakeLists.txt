
set(CRON_TESTS "CronTests")
add_subdirectory(${CRON_TESTS})

set(UTILITIES_TESTS "UtilitiesTests")
add_subdirectory(${UTILITIES_TESTS})

set(LIBRARY_TESTS "tests")
add_library(${LIBRARY_TESTS} INTERFACE "" )

target_link_libraries(${LIBRARY_TESTS}
        INTERFACE
                ${TIMED_ACTION_LIBS}
                ${CRON_TESTS}
                ${UTILITIES_TESTS}
        )

# CMakeLists.txt in the 'tests' directory

# Include Google Test as a subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/google_test
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# Define a custom target to run the tests
add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS ${LIBRARY_TESTS}
)


#if(BUILD_TESTING)
#        set(TEST_EXECUTABLE "TimedActionTestsExe")
#        add_executable(${TEST_EXECUTABLE}
#                ${TIMED_ACTION_LIBS}
#                ${CRON_TESTS}
#                ${UTILITIES_TESTS}
#        )
#        target_link_libraries(${TEST_EXECUTABLE}
#                PRIVATE
#                GTest::gtest_main
#                ${LIBRARY_TESTS})
#        include(GoogleTest)
#        gtest_discover_tests(${TEST_EXECUTABLE})
#endif()


