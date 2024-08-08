cmake_minimum_required(VERSION 3.16.3)
project(strutil VERSION 2.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(my_program ${SOURCES})

find_package(Threads REQUIRED)

target_link_libraries(my_program ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)