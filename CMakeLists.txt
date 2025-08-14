cmake_minimum_required(VERSION 3.10)
project(cgraph C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES "src/*.c")
add_library(cgraph STATIC ${SOURCES})
target_include_directories(cgraph PUBLIC include/public)
target_include_directories(cgraph PRIVATE include/private)

add_executable(cgraph_test src/test/test.c)
target_include_directories(cgraph_test PUBLIC include/public)
target_link_libraries(cgraph_test PUBLIC cgraph)