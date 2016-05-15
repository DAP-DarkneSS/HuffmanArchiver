cmake_minimum_required(VERSION 2.6)
project(huffar C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -Wall -Wextra -std=c99")

add_executable (huffar
    huffcode.c
    compress.c
    extract.c
    main.c
)

install(
    TARGETS huffar
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    COMPONENT binaries
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/huffar.1
    DESTINATION ${SHARE_INSTALL_PREFIX}/man/man1
    COMPONENT doc)
