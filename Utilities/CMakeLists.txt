
set(UTILITY_SOURCES
        "StringUtils.hpp"
        "Logger.hpp"
        "ConfigJSON.hpp")

add_library(${UTILITY_LIBRARY} INTERFACE ${UTILITY_SOURCES} )


target_include_directories( ${UTILITY_LIBRARY}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )