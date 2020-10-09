#ifndef MONTADOR_H
#define MONTADOR_H

#include <map>
#include <string>
#include "utils.hpp"
#include "error.hpp"
#include "ArquivoHandler.hpp"

class Montador {
public:
    ArquivoHandler *arquivo;
    MontadorErrors errors;

    map<string, int> tabelaDeSimbolos = {};
    // Tabela de instrucoes com o nome de instrução/opcode
    map<string, int> tabelaDeIntrucoes = {
            {"ADD",    1},
            {"SUB",    2},
            {"MULT",   3},
            {"DIV",    4},
            {"JMP",    5},
            {"JMPN",   6},
            {"JMPP",   7},
            {"JMPZ",   8},
            {"COPY",   9},
            {"LOAD",   10},
            {"STORE",  11},
            {"INPUT",  12},
            {"OUTPUT", 13},
            {"STOP",   14}
    };
    // Tabela de diretivas com o nome da diretiva e o tamanho da diretiva
    map<string, int> tabelaDeDiretivas = {
            {"SECTION", 0},
            {"SPACE",   1},
            {"CONST",   1},
            {"EQU",     0},
            {"IF",      0}
    };

    explicit Montador(ArquivoHandler *arquivoHandler);

    static int tamInstrucao(const string &instrucao);

    static int isOperacaoValida(const Linha &linha);

    void primeiraPassagem();

    string segundaPassagem();
};

#endif
