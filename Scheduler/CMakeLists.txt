
set(SCHEDULER_SOURCES
        "EventQueue.hpp"
        "TimeTable.hpp"
        "WatchableList.hpp"
        "Watcher.hpp"
        "Scheduler.hpp")

add_library(${LIBRARY_SCHEDULER} INTERFACE ${SCHEDULER_SOURCES} )


target_include_directories( ${LIBRARY_SCHEDULER}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )