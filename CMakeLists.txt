cmake_minimum_required(VERSION 3.15)
project(MontadorSB)

set(CMAKE_CXX_STANDARD 14)

add_executable(Main src/preProcessador.cpp)
add_executable(Trabalhos test/test.cpp)