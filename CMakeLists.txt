cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Tester VERSION 1.0)
configure_file(Config.h.in Config.h)

add_executable(Tester Tester.cpp)
target_include_directories(Tester PUBLIC "${PROJECT_BINARY_DIR}")