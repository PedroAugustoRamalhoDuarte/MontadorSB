#include <cstdlib>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Montador {

public:
    map<string, int> tabelaDeSimbolos = {};
    map<string, int> tabelaDeIntrucoes = {
            {"ADD", 1},
            {"SUB", 2},
            {"MULT", 3},
            {"DIV", 4},
            {"JMP", 5},
            {"JMPN", 6},
            {"JMPP", 7},
            {"JMPZ", 8},
            {"COPY", 9},
            {"LOAD", 10},
            {"STORE", 11},
            {"INPUT", 12},
            {"OUTPUT", 13},
            {"STOP", 14}
    };

    map<string, int> tabelaDeDiretivas = {};
    fstream arquivo;

    explicit Montador(const char *nomeArquivo) {
        arquivo.open(nomeArquivo, ios::in);
        if (!arquivo)
            throw "Arquivo não encontrado";
    }

    int tamInstrucao(string instrucao) {
        return 2;
    }

    void primeiraPassagem() {
        string linha;
        int contador_posicao = 0, contador_linha = 1;
        // Enquanto arquivo fonte não chegou ao fim
        while (arquivo) {
            // Obtém uma linha do fonte
            getline(arquivo, linha);

            // TODO coletar instrucao
            string operacao = "SUB";

            // Se existe rótulo
            int pos = linha.find(':');
            if (string::npos != pos) {
                string rotulo = linha.substr(0, pos);
                if (tabelaDeSimbolos.end() != tabelaDeSimbolos.find(rotulo)) {
                    // Se já existe o simbolo na tabela de simbolos
                    throw "Error -> simbolo redefinido";
                } else {
                    // Se não Insere rótulo e contador_posição na TS
                    tabelaDeSimbolos[rotulo] = contador_posicao + tamInstrucao(operacao);
                }

            }

            // Procura operação na tabela de instruções
            if (tabelaDeIntrucoes.end() != tabelaDeIntrucoes.find(operacao)) {
                contador_posicao += tamInstrucao(operacao);
            } else {
                // Procura operação na tabela de diretivas
                if (tabelaDeDiretivas.end() != tabelaDeDiretivas.find(operacao)) {
                    // TODO Chamar subrotina contador_posição = valor retornado pela subrotina
                } else {
                    throw "Erro -> Operação não identificada";
                }
            }
        }

    }

    void segundaPassagem() {
        // Montar o código
    }
};