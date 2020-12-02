add_executable(lab2
    main.cpp
)

target_link_libraries(lab2 stdc++fs)

install(TARGETS lab2
        RUNTIME
        DESTINATION lab2
        COMPONENT lab2
)

install(DIRECTORY output
        DESTINATION lab2
        COMPONENT lab2
)

