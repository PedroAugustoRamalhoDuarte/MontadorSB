cmake_minimum_required(VERSION 3.15)
project(MontadorSB)

set(CMAKE_CXX_STANDARD 14)

add_executable(Main src/main.cpp)
add_executable(Trabalhos src/montador.cpp test/test.cpp)