cmake_minimum_required(VERSION 4.0)
project(HexOS)

set(CMAKE_CXX_STANDARD 20)

add_executable(HexOS main.cpp
        kernel/idt.c)
