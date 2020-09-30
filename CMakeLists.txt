cmake_minimum_required(VERSION 3.10.2)

project(comp-math2020)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



include_directories(support/include)
add_subdirectory(support)
add_subdirectory(lab1)

