set(SOURCES
    main.c
)

add_executable(evaluate ${SOURCES})

target_link_libraries(evaluate
    PUBLIC
        xinji
)

install(
    TARGETS evaluate
    RUNTIME DESTINATION bin
)
