

# Add CronDebugger library
set(LIBRARY_CRON_DEBUGGER "CronDebugger")
add_subdirectory(${LIBRARY_CRON_DEBUGGER})


set(CRON_SOURCES
    "CronRegex.hpp"
    "LeapYearUtils.hpp"
    "CronPart.hpp"
    "Cron.hpp"
    "CronInterpreter.hpp"
    "CronCapsule.hpp"
    "WeekdayPart.hpp"
    "YearPart.hpp"
    "Sort.hpp")

add_library(${LIBRARY_CRON} INTERFACE ${CRON_SOURCES})


target_include_directories( ${LIBRARY_CRON}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_CRON_DEBUGGER})