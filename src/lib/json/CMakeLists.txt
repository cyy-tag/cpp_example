cmake_minimum_required(VERSION 3.16)
project(test_json)

include_directories(./json/include)

add_executable(test_json src/example.cpp)
add_executable(test_array src/array.cpp)
add_executable(test_object src/object.cpp)
