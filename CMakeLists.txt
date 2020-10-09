cmake_minimum_required(VERSION 3.15)
project(MontadorSB)

set(CMAKE_CXX_STANDARD 14)

add_executable(Main src/main.cpp src/utils.cpp src/ArquivoHandler.cpp src/erro.cpp src/montador.cpp src/preProcessador.cpp )
add_executable(Testes test/test.cpp test/utilsTest.cpp test/preProcessadorTest.cpp src/erro.cpp src/utils.cpp src/ArquivoHandler.cpp src/erro.cpp src/montador.cpp src/preProcessador.cpp )