set(EXECUTION_TIMER_SOURCES
        "ExecutionList.hpp"
        "IntervalExecution.hpp")

add_library(${LIBRARY_EXECUTION_TIMER} INTERFACE ${EXECUTION_TIMER_SOURCES} )


target_include_directories( ${LIBRARY_EXECUTION_TIMER}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )