cmake_minimum_required(VERSION 3.10)
project(CPro)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SRC_FILES
    "start.c"
    "HelloWorld/*.c"
)
add_executable(start ${SRC_FILES})
