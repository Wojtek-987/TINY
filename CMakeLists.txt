cmake_minimum_required(VERSION 3.28)
project(TINY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_CXX_STANDARD 20)

add_executable(TINY main.cpp)
