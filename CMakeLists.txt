cmake_minimum_required(VERSION 3.28)
project(TINY)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_BUILD_TYPE Debug)


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
        VariableManager.cpp
        headers/VariableManager.h
)

target_link_options(TINY PRIVATE -static-libgcc -static-libstdc++)