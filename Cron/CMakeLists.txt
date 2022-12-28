set(CRON_SOURCES
        "CronRegex.hpp"
        "UTC_Parser.hpp"
        "CronPart.hpp"
        "Cron.hpp"
        "CronInterpreter.hpp"
        "CronInterpreter.hpp")

add_library(${LIBRARY_CRON} INTERFACE ${CRON_SOURCES} )


target_include_directories( ${LIBRARY_CRON}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )