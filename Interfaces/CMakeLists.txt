set(INTERFACE_SOURCES
        "I_ExecutionTimer.hpp")

add_library(${LIBRARY_INTERFACES} INTERFACE ${INTERFACE_SOURCES} )


target_include_directories( ${LIBRARY_INTERFACES}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )