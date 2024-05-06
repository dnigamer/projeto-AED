cmake_minimum_required(VERSION 3.27)
project(Projeto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(Boost_USE_STATIC_LIBS ON)
else ()
    set(Boost_USE_STATIC_LIBS OFF)
endif ()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/share")
    set(Qt6_DIR "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/share/Qt6")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

add_executable(Projeto main.c
        src/produtos.c
        src/produtos.h
        src/gui/gui.cpp
        src/gui/gui.h
        src/gui/mainmenu.cpp
        src/gui/mainmenu.h
)

message("-- Linking Qt6")
target_link_libraries(Projeto PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets)
target_include_directories(Projeto PUBLIC ${Qt6_INCLUDE_DIRS})

if (WIN32)
    install(TARGETS Projeto DESTINATION "bin")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
    include(windeployqt)
    windeployqt(Projeto "bin")
endif()