cmake_minimum_required(VERSION 3.29)
project(is_os_tp4 C)

set(CMAKE_C_STANDARD 11)

add_executable(is_os_tp4 main.c
        segdef.h)

target_link_libraries(is_os_tp4 ${CMAKE_SOURCE_DIR}/libseg.a)