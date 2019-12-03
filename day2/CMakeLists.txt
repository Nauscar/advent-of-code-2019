cmake_minimum_required(VERSION 3.12)
project(advent-of-code-2019-day2 C)

set(CMAKE_C_STANDARD 11)

add_executable(day2 main.c)
target_include_directories(day2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Intcode")
target_sources(day2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Intcode/Intcode.c")
target_compile_options(day2 PRIVATE -g)
configure_file(input.in input.in COPYONLY)

add_executable(day2-test test.c)
target_include_directories(day2-test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Intcode")
target_sources(day2-test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Intcode/Intcode.c")
