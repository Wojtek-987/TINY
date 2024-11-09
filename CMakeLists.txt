cmake_minimum_required(VERSION 3.28)
project(TINY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_CXX_STANDARD 20)

add_executable(TINY main.cpp
        Language.cpp
        Tiny.cpp
        types/OutputType.h
        types/Colour.h
        headers/Logger.h
        Logger.cpp
        Run_Loop.cpp
        headers/Run_Loop.h
        TokenQueue.cpp
        headers/TokenQueue.h
        types/Token.h
        Tokenizer.cpp
        headers/Tokenizer.h
)
