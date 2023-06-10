

# Add CronDebugger library
set(LIBRARY_CRON_DEBUGGER "CronDebugger")
add_subdirectory(${LIBRARY_CRON_DEBUGGER})
if (LIBRARY_CRON_DEBUGGER)
    message(NOTICE " - [ ${CRON_SUB_LIBRARY_NUMBER}.1/${SUB_LIB_COUNT} ] ${LIBRARY_CRON_DEBUGGER} found")
else()
    message(FATAL_ERROR " - [ ${CRON_SUB_LIBRARY_NUMBER}.1/${SUB_LIB_COUNT} ] ${LIBRARY_CRON_DEBUGGER} not found")
endif()

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