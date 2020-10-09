#include "catch.hpp"
#include "../include/preProcessador.hpp"

TEST_CASE("IF e EQU", "[preProcessador]") {
    std::vector<std::string> arquivoResultado = {
            "SECTION TEXT",
            "LOAD SPACE",
            "SECTION DATA",
            "N: SPACE"
    };
    PreProcessador preProcessador("../test/files/preProcess.asm", false);
    preProcessador.run();
    auto *arquivoEmMemoria = dynamic_cast<ArquivoEmMemoria *>(preProcessador.arquivoPreProcessado);
    REQUIRE(arquivoEmMemoria->arquivo == arquivoResultado);
}


TEST_CASE("EQU e CONST", "[preProcessador]") {
    std::vector<std::string> arquivoResultado = {
            "SECTION TEXT",
            "STORE N",
            "SECTION DATA",
            "N: CONST 10",
    };
    PreProcessador preProcessador("../test/files/preProcess1.asm", false);
    preProcessador.run();
    auto *arquivoEmMemoria = dynamic_cast<ArquivoEmMemoria *>(preProcessador.arquivoPreProcessado);
    REQUIRE(arquivoEmMemoria->arquivo == arquivoResultado);
}

TEST_CASE("Multi Line Rótulo", "[preProcessador]") {
    std::vector<std::string> arquivoResultado = {
            "SECTION TEXT",
            "L1: STORE N",
            "SECTION DATA",
            "N: CONST 10",
    };
    PreProcessador preProcessador("../test/files/preProcess2.asm", false);
    preProcessador.run();
    auto *arquivoEmMemoria = dynamic_cast<ArquivoEmMemoria *>(preProcessador.arquivoPreProcessado);
    REQUIRE(arquivoEmMemoria->arquivo == arquivoResultado);
}