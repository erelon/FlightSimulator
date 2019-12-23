cmake_minimum_required(VERSION 3.15)
project(untitled3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(untitled3 main.cpp lexer.cpp lexer.h command.h commands.h parser.cpp parser.h clinetserver.h clientserver.cpp)