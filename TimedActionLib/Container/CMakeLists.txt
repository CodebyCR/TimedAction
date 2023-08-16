set(CONTAINER_SOURCES
        "AsyncQueue.hpp"
        "DI_Model.hpp")

add_library(${LIBRARY_CONTAINER} INTERFACE ${CONTAINER_SOURCES} )


target_include_directories( ${LIBRARY_CONTAINER}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )