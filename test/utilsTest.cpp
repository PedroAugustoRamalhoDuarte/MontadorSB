#include "catch.hpp"
#include "../include/utils.hpp"

TEST_CASE("Variavel valida", "[utils]") {
    REQUIRE_FALSE(isVariavelValida(""));
    REQUIRE(isVariavelValida("ABACATE"));
    REQUIRE(isVariavelValida("teste"));
}

TEST_CASE("ColetaTermos da Linha", "[utils]") {
    SECTION("Linha completa") {
        Linha linha = coletaTermosDaLinha("CPY: COPY N1, N4 ;comentario qualquer");
        REQUIRE(linha.rotulo == "CPY");
        REQUIRE(linha.operacao == "COPY");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2 == "N4");
    }SECTION("Linha sem rotúlo") {
        Linha linha = coletaTermosDaLinha("COPY N1, N4 ;comentario qualquer");
        REQUIRE(linha.rotulo.empty());
        REQUIRE(linha.operacao == "COPY");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2 == "N4");
    }SECTION("Linha sem op2") {
        Linha linha = coletaTermosDaLinha("SUB N1");
        REQUIRE(linha.rotulo.empty());
        REQUIRE(linha.operacao == "SUB");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2.empty());
    }SECTION("com tab") {
        Linha linha = coletaTermosDaLinha("   SUB N1");
        REQUIRE(linha.rotulo.empty());
        REQUIRE(linha.operacao == "SUB");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2.empty());
    }
}
