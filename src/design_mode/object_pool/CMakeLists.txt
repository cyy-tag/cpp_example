cmake_minimum_required(VERSION 3.16.3)
project(object_pool)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(EXPORT_COMPILE_COMMANDS ON)
add_executable(object_pool benchmark_objectpool.cpp)
target_link_libraries(object_pool benchmark::benchmark)
