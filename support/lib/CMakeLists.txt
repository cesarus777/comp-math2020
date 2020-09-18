add_library(compute_support SHARED
    gnuplot.cpp
)

install(TARGETS compute_support
        LIBRARY
        DESTINATION support
)

