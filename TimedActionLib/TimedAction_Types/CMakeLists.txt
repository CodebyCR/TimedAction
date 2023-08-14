
set(TIMED_ACTION_SOURCES
        "ScheduledAction.hpp"
        "TimedAction.hpp"
        "Light_TimedAction.hpp")

add_library(${LIBRARY_TIMED_ACTION} INTERFACE ${TIMED_ACTION_SOURCES} )


target_include_directories( ${LIBRARY_TIMED_ACTION}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )