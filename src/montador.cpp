#include <map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

struct Linha {
    string rotulo;
    string operacao;
    string op1;
    string op2;
};

class Montador {

public:
    map<string, int> tabelaDeSimbolos = {};
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

    map<string, int> tabelaDeDiretivas = {
            {"SECTION",  0},
            {"SPACE",    1},
            {"CONST",    1},
            {"EQU",      0},
            {"IF",       0},
            {"MACRO",    0},
            {"ENDMACRO", 0}
    };
    fstream arquivo;

    explicit Montador(const char *nomeArquivo) {
        arquivo.open(nomeArquivo, ios::in);
        if (!arquivo)
            throw "Arquivo não encontrado";
    }

    static int tamInstrucao(const string &instrucao) {
        if (instrucao == "COPY") {
            return 3;
        } else if (instrucao == "STOP" or instrucao == "SPACE" or instrucao == "CONST") {
            return 1;
        } else {
            return 2;
        }
    }

    static Linha coletaTermosDaLinha(const string &linha) {
        string elementos[4];
        int cont = 0;
        bool jaPulou = false;
        // Se não tiver rótulo, começa preenchendo instrução
        if (string::npos == linha.find(':'))
            cont++;

        for (char ch : linha) {
            if (ch == ';') {
                break;
            }
            if (ch == ' ' or ch == ',' or (ch == ':' and cont == 0)) {
                if (!jaPulou)
                    cont++;
                jaPulou = true;
            } else {
                jaPulou = false;
                elementos[cont] += ch;
            }
        }
        Linha l = {elementos[0], elementos[1], elementos[2], elementos[3]};
        return l;
    }

    void primeiraPassagem() {
        string linha;
        int contador_posicao = 0, contador_linha = 1;
        // Enquanto arquivo fonte não chegou ao fim
        while (arquivo) {
            if( arquivo.eof() ) break;
            // Obtém uma linha do fonte
            getline(arquivo, linha);

            // Separa os elementos da linha
            Linha l = coletaTermosDaLinha(linha);

            // Se existe rótulo
            if (!l.rotulo.empty()) {
                if (tabelaDeSimbolos.end() != tabelaDeSimbolos.find(l.rotulo)) {
                    // Se já existe o simbolo na tabela de simbolos
                    throw "Error -> simbolo redefinido";
                } else {
                    // Se não Insere rótulo e contador_posição na TS
                    tabelaDeSimbolos[l.rotulo] = contador_posicao;
                }

            }

            // Procura operação na tabela de instruções
            if (tabelaDeIntrucoes.end() != tabelaDeIntrucoes.find(l.operacao)) {
                contador_posicao += tamInstrucao(l.operacao);
            } else {
                // Procura operação na tabela de diretivas
                if (tabelaDeDiretivas.end() != tabelaDeDiretivas.find(l.operacao)) {
                    contador_posicao += tamInstrucao(l.operacao);
                    // TODO Chamar subrotina contador_posição = valor retornado pela subrotina
                } else {
                    throw "Erro -> Operação não identificada";
                }
            }

            contador_linha += 1;
        }

    }

    void segundaPassagem() {
        // Montar o código
    }
};