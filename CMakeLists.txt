cmake_minimum_required(VERSION 2.6)
project(huff C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -Wall -std=c99")

add_executable(huff main.c)

install(TARGETS huff RUNTIME DESTINATION bin)
