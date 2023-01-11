set(CRON_SOURCES
        "CronRegex.hpp"
        "UTC_Parser.hpp"
        "LeapYearUtils.hpp"
        "CronPart.hpp"
        "Cron.hpp"
        "CronInterpreter.hpp" CronCapsule.hpp)

add_library(${LIBRARY_CRON} INTERFACE ${CRON_SOURCES} )


target_include_directories( ${LIBRARY_CRON}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )