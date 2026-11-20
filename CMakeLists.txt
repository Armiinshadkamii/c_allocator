cmake_minimum_required(VERSION 4.3)
project(c_allocator C)

set(CMAKE_C_STANDARD 11)

add_executable(c_allocator main.c
        allocator/allocator.h
        allocator/allocator.c)
