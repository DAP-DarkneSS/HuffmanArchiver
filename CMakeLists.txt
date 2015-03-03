cmake_minimum_required(VERSION 2.6)
project(huffmanarchiver C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -Wall -std=c99")

add_executable(huffmanarchiver main.c)

install(TARGETS huffmanarchiver RUNTIME DESTINATION bin)
