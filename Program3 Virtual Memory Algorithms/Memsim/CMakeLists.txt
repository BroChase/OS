cmake_minimum_required(VERSION 3.8)
project(Memsim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp memexceptions.h memsim.h)
add_executable(Memsim ${SOURCE_FILES})