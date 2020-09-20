#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/montador.cpp"

TEST_CASE("Table de Simbolos", "[single-file]") {
    Montador montador("../test/files/rotuloDuplicado.asm");

    REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Error -> simbolo redefinido");

}
