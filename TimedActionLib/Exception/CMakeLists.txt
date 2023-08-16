set(EXCEPTION_SOURCES
        "SchedulerException.hpp")

add_library(${LIBRARY_EXCEPTION} INTERFACE ${EXCEPTION_SOURCES} )


target_include_directories( ${LIBRARY_EXCEPTION}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )