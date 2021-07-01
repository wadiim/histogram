cmake_minimum_required(VERSION 3.1)

project(histogram)

set(CMAKE_C_STANDARD 90)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
