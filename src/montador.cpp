#include <map>
#include <string>
#include "utils.cpp"
#include "erro.cpp"
#include "ArquivoHandler.hpp"

using namespace std;

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
    ArquivoHandler *arquivo;

    explicit Montador(ArquivoHandler *arquivoHandler) {
        this->arquivo = arquivoHandler;
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
            } else if (linha.operacao == "SECTION") {
                isValida = isValida and (linha.op1 == "TEXT" or linha.op1 == "DATA");
            }
            return isValida;
        }
    }

    void primeiraPassagem() {
        string linha;
        bool isInsideValid = true;
        int contador_posicao = 0, contador_linha = 1;
        // Enquanto arquivo fonte não chegou ao fim
        while (!arquivo->hasEnd()) {
            // Obtém uma linha do fonte
            arquivo->getLine(&linha);

            // Separa os elementos da linha
            Linha l = coletaTermosDaLinha(linha);

            // Se existe rótulo
            if (!l.rotulo.empty()) {
                if (tabelaDeSimbolos.end() != tabelaDeSimbolos.find(l.rotulo)) {
                    // Se já existe o simbolo na tabela de simbolos
                    throw MontadorErro("Símbolo Redefinido", "TIPO", linha, contador_linha);
                } else {
                    // Se não Insere rótulo e contador_posição na TS
                    if (l.operacao == "EQU") {
                        // Na Diretiva EQU guardar o valor da operação
                        tabelaDeSimbolos[l.rotulo] = stoi(l.op1);
                    } else {
                        tabelaDeSimbolos[l.rotulo] = contador_posicao;
                    }

                }

            }

            // Procura operação na tabela de instruções
            if (tabelaDeIntrucoes.end() != tabelaDeIntrucoes.find(l.operacao)) {
                contador_posicao += tamInstrucao(l.operacao);
            } else {
                // Procura operação na tabela de diretivas
                if (tabelaDeDiretivas.end() != tabelaDeDiretivas.find(l.operacao)) {
                    contador_posicao += tamInstrucao(l.operacao);
                } else {
                    throw MontadorErro("Operação não identificada", "TIPO", linha, contador_linha);
                }
            }

            contador_linha += 1;
        }
        // Voltar arquivo para o começo
        arquivo->resetFile();
    }

    string segundaPassagem() {
        string linha;
        string code;
        int contador_posicao = 0, contador_linha = 1;
        // Enquanto arquivo fonte não chegou ao fim
        while (!arquivo->hasEnd()) {
            // Obtém uma linha do fonte
            arquivo->getLine(&linha);

            // Separa os elementos da linha
            Linha l = coletaTermosDaLinha(linha);

            // Para cada operando
            if (l.operacao != "CONST" and l.operacao != "SECTION") {
                if ((tabelaDeSimbolos.end() == tabelaDeSimbolos.find(l.op1) and !l.op1.empty()) or (
                        tabelaDeSimbolos.end() == tabelaDeSimbolos.find(l.op2) and !l.op2.empty())) {
                    throw MontadorErro("Símobolo indefinido", "TIPO", linha, contador_linha);
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
                    throw MontadorErro("Operando inválido", "TIPO", linha, contador_linha);
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
                    throw MontadorErro("Operando não identificada", "TIPO", linha, contador_linha);
                }
            }
            contador_linha += 1;
        }
        return code;
    }

};