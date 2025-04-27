cmake_minimum_required(VERSION 3.16)
project(functional)
set(CMAKE_CXX_STANDARD 20)

add_executable(function functional.cpp)
target_link_libraries(function benchmark::benchmark)
