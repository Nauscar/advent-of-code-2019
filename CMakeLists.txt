cmake_minimum_required(VERSION 3.10)

project(advent-of-code-2019 C)

message(STATUS "${CMAKE_MODULE_PATH}")
message(STATUS "Using C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Using C Compiler: ${CMAKE_C_COMPILER}")

add_subdirectory(day1)
add_subdirectory(day2)