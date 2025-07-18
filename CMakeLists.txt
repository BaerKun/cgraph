cmake_minimum_required(VERSION 3.10)
project(graph C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES "src/*.c")
add_library(graph STATIC ${SOURCES})
target_include_directories(graph PUBLIC include/public)
target_include_directories(graph PRIVATE include/private)

add_executable(test_graph src/test/test.c)
target_include_directories(test_graph PUBLIC include/public)
target_link_libraries(test_graph PUBLIC graph)