cmake_minimum_required(VERSION 3.16.3)
project(asio)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(EXPORT_COMPILE_COMMANDS ON)
include_directories(./asio/asio/include)

link_libraries(pthread uring)
add_subdirectory(src/asio_file)

add_executable(asio src/main.cpp)