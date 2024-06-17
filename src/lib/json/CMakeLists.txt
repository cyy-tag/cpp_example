cmake_minimum_required(VERSION 3.16)
project(test_json)

include_directories(./json/include)

add_executable(test_json example.cpp)
