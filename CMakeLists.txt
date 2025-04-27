cmake_minimum_required(VERSION 3.16)
project(cpp_example)
set(CMAKE_CXX_STANDARD 20)
add_definitions(
                -DCHECK_PTHREAD_RETURN_VALUE
                -D_FILE_OFFSET_BITS=64
                -DASIO_HAS_IO_URING
                -Wall
                -O3
								-g
)
add_subdirectory(third/googletest)
add_subdirectory(third/benchmark)
include_directories(
            third/googletest/googletest/include
            third/googletest/googlemock/include
            third/benchmark/include
            src/lib/asio/asio/asio/include
            src/
            /usr/local/src/proxy)


link_libraries(gtest_main gtest benchmark::benchmark)
#compiler_command.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/design_mode/object_pool)
# add_subdirectory(src/lib/boost)
# add_subdirectory(src/lib/asio)
# add_subdirectory(src/grammar_example/chrono)
add_subdirectory(src/grammar_example/functional)
