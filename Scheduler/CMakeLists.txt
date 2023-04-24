
set(SCHEDULER_SOURCES
        "EventQueue.hpp"
        "Watcher.hpp"
        "JobMap.hpp"
        "Scheduler.hpp")

add_library(${LIBRARY_SCHEDULER} INTERFACE ${SCHEDULER_SOURCES} )


target_include_directories( ${LIBRARY_SCHEDULER}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )