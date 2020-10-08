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

bool compareFiles(const std::string& filename1, const std::string& filename2)
{
    std::ifstream file1(filename1, std::ifstream::ate | std::ifstream::binary); //open file at the end
    std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary); //open file at the end
    const std::ifstream::pos_type fileSize = file1.tellg();

    if (fileSize != file2.tellg()) {
        return false; //different file size
    }

    file1.seekg(0); //rewind
    file2.seekg(0); //rewind

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    return std::equal(begin1,std::istreambuf_iterator<char>(),begin2); //Second argument is end-of-range iterator
}

TEST_CASE("Pré Processamento") {
    SECTION("IF e EQU") {
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
    }SECTION("EQU e CONST") {
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
    }SECTION("MULTI LINE ROTULO") {
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

}


TEST_CASE("Primeira Passagem Erros", "Montador") {

    SECTION("Rótulo Duplicado") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/rotuloDuplicado.asm");
        Montador montador(arquivoFisico);
        REQUIRE_THROWS(montador.primeiraPassagem());
        // REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Error -> simbolo redefinido");
    }SECTION("Operação não identificada") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/operacaoInexistente.asm");
        Montador montador(arquivoFisico);
        REQUIRE_THROWS(montador.primeiraPassagem());
        //REQUIRE_THROWS_WITH(montador.primeiraPassagem(), "Erro -> Operação não identificada");
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


TEST_CASE("Main") {
    SECTION("Sem parametros") {
        PreProcessador preProcessador("../test/files/bin.asm", false);
        preProcessador.run();
        Montador montador(preProcessador.arquivoPreProcessado);
        montador.primeiraPassagem();
        REQUIRE(montador.segundaPassagem() ==
                "12 29 10 29 4 28 11 30 3 28 11 31 10 29 2 31 11 31 13 31 9 30 29 10 29 7 4 14 2 0 0 0 ");
    }
    SECTION("With -p") {
        PreProcessador preProcessador("../test/files/preProcess1.asm", true);
        preProcessador.run();
        // REQUIRE(compareFiles("../test/files/preProcess1.pre", "../test/files/result/preProcess1.pre"));
    }
    SECTION("With -c") {
        auto *arquivoFisico = new ArquivoFisico("../test/files/bin.asm");
        Montador montador(arquivoFisico);
        montador.primeiraPassagem();
        REQUIRE(montador.segundaPassagem() ==
                "12 29 10 29 4 28 11 30 3 28 11 31 10 29 2 31 11 31 13 31 9 30 29 10 29 7 4 14 2 0 0 0 ");
    }

}