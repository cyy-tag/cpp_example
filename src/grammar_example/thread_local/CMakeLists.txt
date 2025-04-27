cmake_minimum_required(VERSION 3.16)
project(chrono)
set(CMAKE_CXX_STANDARD 20)

add_executable(benchmark_threadlocal benchmark.cpp)
target_link_libraries(benchmark_threadlocal benchmark::benchmark)
