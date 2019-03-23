cmake_minimum_required(VERSION 2.8)

project(dupplicates)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(dupplicates main.cpp mainwindow.h mainwindow.cpp Dupplicator.cpp Dupplicator.h)
qt5_use_modules(dupplicates Widgets)
target_link_libraries(dupplicates OpenSSL::SSL)