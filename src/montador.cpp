#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "utils.cpp"
#include "erro.cpp"

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
        } else if (instrucao == "SECTION" or instrucao == "EQU" or instrucao == "IF") {
            return 0;
        } else {
            return 2;
        }
    };

    // TODO testar essa funcao
    static int isOperacaoValida(const Linha &linha) {
        if (linha.operacao == "COPY") {
            return !linha.op1.empty() and !linha.op2.empty();
        } else if (linha.operacao == "STOP") {
            return linha.op1.empty() and linha.op2.empty();
        } else {
            bool isValida = !linha.op1.empty() and linha.op2.empty();
            if (linha.operacao == "CONST") {
                // Validação da operação CONST (#003)
                isValida = isValida and isInteger(linha.op1);
            }
            return isValida;
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
                // Ignora os comentários (#004)
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
        // Transformar todos os elementos em caixa alta #0001
        Linha l = {toUpperCase(elementos[0]),
                   toUpperCase(elementos[1]),
                   toUpperCase(elementos[2]),
                   toUpperCase(elementos[3])};
        return l;
    }

    void primeiraPassagem() {
        string linha;
        int contador_posicao = 0, contador_linha = 1;
        // Enquanto arquivo fonte não chegou ao fim
        while (arquivo) {
            if (arquivo.eof()) break;
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
        // Voltar arquivo para o começo
        arquivo.clear();
        arquivo.seekg(0, ios::beg);
    }

    string segundaPassagem() {
        string linha;
        string code;
        int contador_posicao = 0, contador_linha = 1;
        // Enquanto arquivo fonte não chegou ao fim
        while (arquivo) {
            if (arquivo.eof()) break;
            // Obtém uma linha do fonte
            getline(arquivo, linha);

            // Separa os elementos da linha
            Linha l = coletaTermosDaLinha(linha);

            // Para cada operando
            if (l.operacao != "CONST" and l.operacao != "SECTION") {
                if ((tabelaDeSimbolos.end() == tabelaDeSimbolos.find(l.op1) and !l.op1.empty()) or (
                        tabelaDeSimbolos.end() == tabelaDeSimbolos.find(l.op2) and !l.op2.empty())) {
                    throw "Erro -> Símbolo indefino";
                }
            }
            // Procura operação na tabela de instruções
            if (tabelaDeIntrucoes.end() != tabelaDeIntrucoes.find(l.operacao)) {
                contador_posicao += tamInstrucao(l.operacao);
                if (isOperacaoValida(l)) {
                    code += to_string(tabelaDeIntrucoes[l.operacao]) + ' ';
                    if (!l.op1.empty()) {
                        code += to_string(tabelaDeSimbolos[l.op1]) + ' ';
                    }
                    if (!l.op2.empty()) {
                        code += to_string(tabelaDeSimbolos[l.op2]) + ' ';
                    }
                } else {
                    throw "Erro -> Operando inválido";
                }

            } else {
                if (tabelaDeDiretivas.end() != tabelaDeDiretivas.find(l.operacao)) {
                    if (l.operacao == "CONST") {
                        code += l.op1 + ' ';
                    } else if (l.operacao == "SPACE") {
                        code += "0 ";
                    }
                    // Chama subrotina que executa a diretiva
                    // Contador_posição = valor retornado pela subrotina
                } else {
                    throw "Erro -> operação não identificada";
                }
            }
            contador_linha += 1;
        }
        return code;
    }

};