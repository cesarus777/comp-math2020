add_executable(lab1 
    main.cpp
    functions.cpp
)

target_link_libraries(lab1 compute_support stdc++fs)

install(TARGETS lab1
        RUNTIME
        DESTINATION lab1
        COMPONENT lab1
)

install(FILES
          plot_to_jpg.gnu
          plot_to_png.gnu
          plot_to_window.gnu
        DESTINATION lab1
        COMPONENT lab1
)

install(DIRECTORY output
        DESTINATION lab1
        COMPONENT lab1
)

