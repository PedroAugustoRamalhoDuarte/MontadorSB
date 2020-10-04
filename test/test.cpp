#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/montador.cpp"
#include "../src/preProcessador.cpp"

template<typename Map>
bool map_compare(Map const &lhs, Map const &rhs) {
    return lhs.size() == rhs.size()
           && std::equal(lhs.begin(), lhs.end(),
                         rhs.begin());
}

TEST_CASE("Pré Processamento") {
    PreProcessador preProcessador("../test/files/preProcess.asm", true);
    preProcessador.run();
}


TEST_CASE("Primeira Passagem Erros", "Montador") {

    SECTION("Rótulo Duplicado") {
        Montador montador("../test/files/rotuloDuplicado.asm");
        REQUIRE_THROWS(montador.primeiraPassagem());
        // REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Error -> simbolo redefinido");
    }SECTION("Operação não identificada") {
        Montador montador("../test/files/operacaoInexistente.asm");
        REQUIRE_THROWS(montador.primeiraPassagem());
        //REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Erro -> Operação não identificada");
    }
}

TEST_CASE("Primeira Passagem sem Erros", "") {
    SECTION("Exemplo1") {
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
    }SECTION("Exemplo 2") {
        Montador montador("../test/files/exemplo2.asm");
        montador.primeiraPassagem();

        CHECK(montador.tabelaDeSimbolos["FRONT"] == 10);
        CHECK(montador.tabelaDeSimbolos["FINAL"] == 30);
        CHECK(montador.tabelaDeSimbolos["ZERO"] == 33);
        CHECK(montador.tabelaDeSimbolos["ONE"] == 34);
        CHECK(montador.tabelaDeSimbolos["OLDER"] == 35);
        CHECK(montador.tabelaDeSimbolos["OLD"] == 36);
        CHECK(montador.tabelaDeSimbolos["NEW"] == 37);
        CHECK(montador.tabelaDeSimbolos["LIMIT"] == 38);
    }SECTION("Exemplo 3") {
        Montador montador("../test/files/exemplo3.asm");
        montador.primeiraPassagem();

        CHECK(montador.tabelaDeSimbolos["FAT"] == 4);
        CHECK(montador.tabelaDeSimbolos["FIM"] == 18);
        CHECK(montador.tabelaDeSimbolos["AUX"] == 21);
        CHECK(montador.tabelaDeSimbolos["N"] == 22);
        CHECK(montador.tabelaDeSimbolos["ONE"] == 23);
    }
}

TEST_CASE("Segunda Passagem sem erros", "") {
    SECTION("bin") {
        Montador montador("../test/files/bin.asm");
        montador.primeiraPassagem();

        REQUIRE(montador.segundaPassagem() ==
                "12 29 10 29 4 28 11 30 3 28 11 31 10 29 2 31 11 31 13 31 9 30 29 10 29 7 4 14 2 0 0 0 ");
    }
}


TEST_CASE("ColetaTermos da Linha", "Montador::colataTermosDaLinha") {
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
    }
}