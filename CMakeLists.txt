cmake_minimum_required(VERSION 3.7)
project(N_Queens)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp vectorVersion.cpp)
add_executable(N_Queens ${SOURCE_FILES})