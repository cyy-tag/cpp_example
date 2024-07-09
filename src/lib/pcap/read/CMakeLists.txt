cmake_minimum_required(VERSION 3.16.3)
project(pcap_read)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(EXPORT_COMPILE_COMMANDS ON)

link_libraries(pcap)

add_executable(pcap_read main.cpp pcap_file.cpp)
