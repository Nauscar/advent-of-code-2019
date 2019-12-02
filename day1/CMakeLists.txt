cmake_minimum_required(VERSION 3.12)
project(advent-of-code-2019-day1 C)

set(CMAKE_C_STANDARD 11)

add_executable(day1 main.c)
target_include_directories(day1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/FuelCounterUpper")
target_sources(day1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/FuelCounterUpper/FuelCounterUpper.c")
configure_file(input.in input.in COPYONLY)

add_executable(day1-test test.c)
target_include_directories(day1-test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/FuelCounterUpper")
target_sources(day1-test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/FuelCounterUpper/FuelCounterUpper.c")
configure_file(test.in test.in COPYONLY)