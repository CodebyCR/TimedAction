
set(TIMED_ACTION_SOURCES
        "../Interfaces/I_TimedAction.hpp"
        "TimedAction.hpp"
        "Light_TimedAction.hpp")

add_library(${LIBRARY_TIMED_ACTION} INTERFACE ${TIMED_ACTION_SOURCES} )


target_include_directories( ${LIBRARY_TIMED_ACTION}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )