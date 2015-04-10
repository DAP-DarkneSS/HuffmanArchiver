cmake_minimum_required(VERSION 2.6)
project(huffar C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99")

add_executable (huffar
    huffcode.c
    compress.c
    extract.c
    main.c
)

install(TARGETS huffar DESTINATION ${CMAKE_INSTALL_PREFIX}/bin COMPONENT binaries)
