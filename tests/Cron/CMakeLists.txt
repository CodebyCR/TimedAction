# Cron Tests
set(CRON_TESTS_SOURCES
        "CronTest.cpp")

add_library(${CRON_TESTS} INTERFACE ${CRON_TESTS_SOURCES} )


target_include_directories( ${CRON_TESTS}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )