cmake_minimum_required(VERSION 3.16)
project(CMakeHelper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CMakeHelper
    src/main.cpp
    src/ProjectScanner.cpp
    src/CMakeGenerator.cpp
)

target_include_directories(CMakeHelper PRIVATE include)
