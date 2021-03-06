#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../include/montador.hpp"
#include "../include/preProcessador.hpp"

template<typename Map>
bool map_compare(Map const &lhs, Map const &rhs) {
    return lhs.size() == rhs.size()
           && std::equal(lhs.begin(), lhs.end(),
                         rhs.begin());
}


TEST_CASE("Primeira Passagem Erros", "Montador") {
    SECTION("Rótulo Duplicado") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/rotuloDuplicado.asm");
        Montador montador(arquivoFisico);
        montador.primeiraPassagem();
        REQUIRE(montador.errors.contemErrors());
    }SECTION("Operação não identificada") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/operacaoInexistente.asm");
        Montador montador(arquivoFisico);
        montador.primeiraPassagem();
        REQUIRE(montador.errors.contemErrors());
    }
}

TEST_CASE("Primeira Passagem sem Erros", "") {
    SECTION("Exemplo1") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/exemplo1.asm");
        Montador montador(arquivoFisico);
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
        auto *arquivoFisico = new ArquivoFisico("../test/files/exemplo2.asm");
        Montador montador(arquivoFisico);
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
        auto *arquivoFisico = new ArquivoFisico("../test/files/exemplo3.asm");
        Montador montador(arquivoFisico);
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
        auto *arquivoFisico = new ArquivoFisico("../test/files/bin.asm");
        Montador montador(arquivoFisico);
        montador.primeiraPassagem();
        REQUIRE(montador.segundaPassagem() ==
                "12 29 10 29 4 28 11 30 3 28 11 31 10 29 2 31 11 31 13 31 9 30 29 10 29 7 4 14 2 0 0 0 ");
    }SECTION("exemplo3") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/exemplo3.asm");
        Montador montador(arquivoFisico);
        montador.primeiraPassagem();

        REQUIRE(montador.segundaPassagem() ==
                "12 22 10 22 2 23 8 18 11 21 3 22 11 22 10 21 6 4 13 22 14 0 0 1 ");
    }
}


TEST_CASE("Main") {
    SECTION("Sem parametros") {
        PreProcessador preProcessador("../test/files/bin.asm", false);
        preProcessador.run();
        Montador montador(preProcessador.arquivoPreProcessado);
        montador.primeiraPassagem();
        REQUIRE(montador.segundaPassagem() ==
                "12 29 10 29 4 28 11 30 3 28 11 31 10 29 2 31 11 31 13 31 9 30 29 10 29 7 4 14 2 0 0 0 ");
    }SECTION("With -p") {
        PreProcessador preProcessador("../test/files/preProcess1.asm", true);
        preProcessador.run();
        // REQUIRE(compareFiles("../test/files/preProcess1.pre", "../test/files/result/preProcess1.pre"));
    }SECTION("With -c") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/bin.asm");
        Montador montador(arquivoFisico);
        montador.primeiraPassagem();
        REQUIRE(montador.segundaPassagem() ==
                "12 29 10 29 4 28 11 30 3 28 11 31 10 29 2 31 11 31 13 31 9 30 29 10 29 7 4 14 2 0 0 0 ");
    }

}