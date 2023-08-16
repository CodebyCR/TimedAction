set(UTILITY_LIBRARY "Utilities")
add_subdirectory(${UTILITY_LIBRARY})

set(LIBRARY_INTERFACES "Interfaces")
add_subdirectory(${LIBRARY_INTERFACES})

set(LIBRARY_EXCEPTION "Exception")
add_subdirectory(${LIBRARY_EXCEPTION})

set(LIBRARY_CRON "Cron")
add_subdirectory(${LIBRARY_CRON})

set(LIBRARY_TIMED_ACTION "TimedAction_Types")
add_subdirectory(${LIBRARY_TIMED_ACTION})

set(LIBRARY_CONTAINER "Container")
add_subdirectory(${LIBRARY_CONTAINER})

set(LIBRARY_EXECUTION_TIMER "ExecutionTimer")
add_subdirectory(${LIBRARY_EXECUTION_TIMER})

set(LIBRARY_SCHEDULER "Scheduler")
add_subdirectory(${LIBRARY_SCHEDULER})

set(LIBRARY_NOTIFICATION "Notification")
add_subdirectory(${LIBRARY_NOTIFICATION})


add_library(${TIMED_ACTION_LIBS} INTERFACE "" )

target_link_libraries(${TIMED_ACTION_LIBS}
        INTERFACE
        ${UTILITY_LIBRARY}
        ${LIBRARY_INTERFACES}
        ${LIBRARY_CRON}
        ${LIBRARY_TIMED_ACTION}
        ${LIBRARY_CONTAINER}
        ${LIBRARY_EXECUTION_TIMER}
        ${LIBRARY_SCHEDULER}
        ${LIBRARY_NOTIFICATION}
        )

install(TARGETS ${TIMED_ACTION_LIBS}
        ARCHIVE DESTINATION lib)
