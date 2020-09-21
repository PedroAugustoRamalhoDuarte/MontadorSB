#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/montador.cpp"


TEST_CASE("Primeira Passagem", "Montador") {
    SECTION("Rótulo Duplicado") {
        Montador montador("../test/files/rotuloDuplicado.asm");

        REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Error -> simbolo redefinido");
    }
    SECTION("Operação não identificada") {
        Montador montador("../test/files/operacaoInexistente.asm");

        REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Erro -> Operação não identificada");
    }
}


TEST_CASE("ColetaTermos da Linha", "Montador::colataTermosDaLinha") {
    SECTION("Linha completa") {
        Linha linha = Montador::coletaTermosDaLinha("CPY: COPY N1, N4 ;comentario qualquer");
        REQUIRE(linha.rotulo == "CPY");
        REQUIRE(linha.operacao == "COPY");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2 == "N4");
    }
    SECTION("Linha sem rotúlo") {
        Linha linha = Montador::coletaTermosDaLinha("COPY N1, N4 ;comentario qualquer");
        REQUIRE(linha.rotulo.empty());
        REQUIRE(linha.operacao == "COPY");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2 == "N4");
    }
    SECTION("Linha sem op2") {
        Linha linha = Montador::coletaTermosDaLinha("SUB N1");
        REQUIRE(linha.rotulo.empty());
        REQUIRE(linha.operacao == "SUB");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2.empty());
    }
}