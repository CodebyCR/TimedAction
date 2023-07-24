set(NOTIFICATION_SOURCES
        "Message.hpp"
        "JobLog.hpp")

add_library(${LIBRARY_NOTIFICATION} INTERFACE ${NOTIFICATION_SOURCES} )


target_include_directories( ${LIBRARY_NOTIFICATION}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )