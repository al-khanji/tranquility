project(tranquility)
cmake_minimum_required(VERSION 2.6)

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

find_package(Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set(CMAKE_COLOR_MAKEFILE ON)


add_subdirectory(src)
