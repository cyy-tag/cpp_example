#https://fmt.dev/latest/usage.html#building-the-library
cmake_minimum_required(VERSION 3.16)
project(test_fmt)
include_directories(fmt/include)
add_subdirectory(fmt)
add_executable(test_fmt src/fmt.cpp)
target_link_libraries(test_fmt fmt::fmt)