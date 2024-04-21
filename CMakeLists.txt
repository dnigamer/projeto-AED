cmake_minimum_required(VERSION 3.27)
project(Projeto C)

set(CMAKE_C_STANDARD 11)

add_executable(Projeto main.c
        src/visual.c
        src/produtos.c
        src/iohelp.c)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -j 6")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -j 6")