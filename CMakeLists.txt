cmake_minimum_required(VERSION 3.17.0)

project(utility VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(6_1 src/6_1.c)
target_link_libraries(6_1 Threads::Threads)

add_executable(6_2 src/6_2.c)
target_link_libraries(6_2 Threads::Threads)