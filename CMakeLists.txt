cmake_minimum_required(VERSION 3.21)
project(fl_studio_plugin_organizer)

set(CMAKE_CXX_STANDARD 20)

add_executable(fl_studio_plugin_organizer main.cpp read.cpp read.h common.h write.cpp write.h)
