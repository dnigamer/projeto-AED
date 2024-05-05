cmake_minimum_required(VERSION 3.27)
project(Projeto)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.2.4/mingw_64/lib/cmake")
    set(QT6_DIR "C:/Qt/6.2.4/mingw_64/lib/cmake/Qt6")
endif()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Projeto main.c
        src/produtos.c
        src/gui/gui.cpp
        src/gui/gui.h
        src/gui/mainmenu.cpp
)

target_link_libraries(Projeto
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()