#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/montador.cpp"

template<typename Map>
bool map_compare(Map const &lhs, Map const &rhs) {
    return lhs.size() == rhs.size()
           && std::equal(lhs.begin(), lhs.end(),
                         rhs.begin());
}

TEST_CASE("Primeira Passagem Erros", "Montador") {
    SECTION("Rótulo Duplicado") {
        Montador montador("../test/files/rotuloDuplicado.asm");

        REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Error -> simbolo redefinido");
    }SECTION("Operação não identificada") {
        Montador montador("../test/files/operacaoInexistente.asm");

        REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Erro -> Operação não identificada");
    }
}

TEST_CASE("Primeira Passagem sem Erros", "") {
    Montador montador("../test/files/exemplo1.asm");
    montador.primeiraPassagem();
    map<string, int> tabelaDeSimbolos = {
            {"N1", 13},
            {"N2", 14},
            {"N3", 15},
    };
    CHECK(montador.tabelaDeSimbolos["N1"] == 13);
    CHECK(montador.tabelaDeSimbolos["N2"] == 14);
    CHECK(montador.tabelaDeSimbolos["N3"] == 15);
    REQUIRE(map_compare(montador.tabelaDeSimbolos, tabelaDeSimbolos));
}

TEST_CASE("ColetaTermos da Linha", "Montador::colataTermosDaLinha") {
    SECTION("Linha completa") {
        Linha linha = Montador::coletaTermosDaLinha("CPY: COPY N1, N4 ;comentario qualquer");
        REQUIRE(linha.rotulo == "CPY");
        REQUIRE(linha.operacao == "COPY");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2 == "N4");
    }SECTION("Linha sem rotúlo") {
        Linha linha = Montador::coletaTermosDaLinha("COPY N1, N4 ;comentario qualquer");
        REQUIRE(linha.rotulo.empty());
        REQUIRE(linha.operacao == "COPY");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2 == "N4");
    }SECTION("Linha sem op2") {
        Linha linha = Montador::coletaTermosDaLinha("SUB N1");
        REQUIRE(linha.rotulo.empty());
        REQUIRE(linha.operacao == "SUB");
        REQUIRE(linha.op1 == "N1");
        REQUIRE(linha.op2.empty());
    }
}