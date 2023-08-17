set(CRON_TESTS "Cron/CronTest.cpp")
add_subdirectory(${CRON_TESTS})

set(TIMED_ACTION_TESTS "tests")
add_library(${TIMED_ACTION_TESTS} INTERFACE "" )

target_link_libraries(${TIMED_ACTION_TESTS}
        INTERFACE
            ${TIMED_ACTION_LIBS}
            ${CRON_TESTS}
        )