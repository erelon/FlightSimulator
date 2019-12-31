cmake_minimum_required(VERSION 3.15)
project(untitled3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(untitled3 main.cpp lexer.cpp lexer.h clientserver.h clientserver.cpp command.h commands.h parser.cpp parser.h calc/shutting_yard.cpp calc/shutting_yard.h calc/Expression.h calc/claculator.cpp calc/claculator.h utils.h utils.cpp commands.cpp)