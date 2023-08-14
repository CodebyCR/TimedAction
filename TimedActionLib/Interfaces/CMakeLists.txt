set(INTERFACE_SOURCES
        "Message.hpp"
        "ActionCapsule.hpp"
        "Watchable.hpp"
        "I_ExecutionTimer.hpp"
        "I_TimedAction.hpp"
        )

add_library(${LIBRARY_INTERFACES} INTERFACE ${INTERFACE_SOURCES} )


target_include_directories( ${LIBRARY_INTERFACES}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR} )