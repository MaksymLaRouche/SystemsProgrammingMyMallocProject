cmake_minimum_required(VERSION 4.2)
project(P1 C)

set(CMAKE_C_STANDARD 99)

add_executable(SystemsProgrammingMyMallocProject
        mymalloc.h
        mymalloc.c
        memgrind.c)
