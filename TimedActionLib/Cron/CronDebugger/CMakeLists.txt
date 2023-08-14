
set(CRON_DEBUGGER_SOURCES
        "src/CronRanges.hpp"
        "tests/CronRangesTests.hpp")

add_library(${LIBRARY_CRON_DEBUGGER} INTERFACE ${CRON_DEBUGGER_SOURCES} )


target_include_directories( ${LIBRARY_CRON_DEBUGGER}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )