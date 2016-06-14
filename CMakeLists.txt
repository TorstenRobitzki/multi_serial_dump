cmake_minimum_required(VERSION 2.8.12)

add_compile_options(-std=c++11 -Wall -pedantic -Wno-logical-op-parentheses)

add_executable(multi_serial_dump multi_serial_dump.cpp)