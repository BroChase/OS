cmake_minimum_required(VERSION 3.8)
project(disksim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp diskexceptions.h disksim.h)
add_executable(disksim ${SOURCE_FILES})