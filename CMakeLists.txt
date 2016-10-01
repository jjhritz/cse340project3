cmake_minimum_required(VERSION 3.6)
project(cse340project3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES project3.cpp lexer.c lexer.h project3.h)
add_executable(cse340project3 ${SOURCE_FILES})