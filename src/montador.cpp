#ifndef MONTADOR
#define MONTADOR

#include "../include/montador.hpp"

using namespace std;

Montador::Montador(ArquivoHandler *arquivoHandler) {
    this->arquivo = arquivoHandler;
}

int Montador::tamInstrucao(const string &instrucao) {
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

int Montador::isOperacaoValida(const Linha &linha) {
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

void Montador::primeiraPassagem() {
    string linha;
    int contadorPosicao = 0, contadorLinha = 0;
    while (!arquivo->hasEnd()) {
        try {
            arquivo->getLine(&linha);
            contadorLinha += 1;
            Linha l = coletaTermosDaLinha(linha);

            if (contadorLinha == 1 and (l.operacao != "SECTION" or l.op1 != "TEXT")) {
                throw MontadorException(MontadorException::TEXT_FALTANTE);
            }

            // Se existe rótulo
            if (!l.rotulo.empty()) {
                if (tabelaDeSimbolos.end() != tabelaDeSimbolos.find(l.rotulo)) {
                    // Se já existe o simbolo na tabela de simbolos
                    throw MontadorException(MontadorException::ROTULO_REPETIDO);
                } else {
                    // Se não Insere rótulo e contador_posição na TS
                    if (l.operacao == "EQU") {
                        // Na Diretiva EQU guardar o valor da operação
                        tabelaDeSimbolos[l.rotulo] = stoi(l.op1);
                    } else {
                        tabelaDeSimbolos[l.rotulo] = contadorPosicao;
                    }
                }
            }

            // Procura operação na tabela de instruções
            if (tabelaDeIntrucoes.end() != tabelaDeIntrucoes.find(l.operacao)) {
                contadorPosicao += tamInstrucao(l.operacao);
            } else {
                // Procura operação na tabela de diretivas
                if (tabelaDeDiretivas.end() != tabelaDeDiretivas.find(l.operacao)) {
                    contadorPosicao += tamInstrucao(l.operacao);
                } else {
                    throw MontadorException(MontadorException::OPERACAO_INVALIDA);
                }
            }
        } catch (MontadorException &e) {
            errors.adicionaError(e.error, linha, contadorLinha);
            continue;
        }
    }

    if (errors.contemErrors()) {
        throw PassagemException("Primeira Passagem", &errors);
    }
    // Voltar arquivo para o começo
    arquivo->resetFile();
}

string Montador::segundaPassagem() {
    string linha;
    string code;
    int contadorPosicao = 0, contadorLinha = 1;
    while (!arquivo->hasEnd()) {
        try {
            arquivo->getLine(&linha);
            contadorLinha += 1;
            Linha l = coletaTermosDaLinha(linha);

            // Para cada operando
            if (l.operacao != "CONST" and l.operacao != "SECTION") {
                if ((tabelaDeSimbolos.end() == tabelaDeSimbolos.find(l.op1) and !l.op1.empty()) or (
                        tabelaDeSimbolos.end() == tabelaDeSimbolos.find(l.op2) and !l.op2.empty())) {
                    throw MontadorException(MontadorException::ROTULO_AUSENTE);
                }
            }
            // Procura operação na tabela de instruções
            if (tabelaDeIntrucoes.end() != tabelaDeIntrucoes.find(l.operacao)) {
                contadorPosicao += tamInstrucao(l.operacao);
                if (isOperacaoValida(l)) {
                    code += to_string(tabelaDeIntrucoes[l.operacao]) + ' ';
                    if (!l.op1.empty()) {
                        code += to_string(tabelaDeSimbolos[l.op1]) + ' ';
                    }
                    if (!l.op2.empty()) {
                        code += to_string(tabelaDeSimbolos[l.op2]) + ' ';
                    }
                } else {
                    throw MontadorException(MontadorException::OPERANDO_INVALIDO);
                }

            } else {
                if (tabelaDeDiretivas.end() != tabelaDeDiretivas.find(l.operacao)) {
                    if (l.operacao == "CONST") {
                        code += l.op1 + ' ';
                    } else if (l.operacao == "SPACE") {
                        code += "0 ";
                    }
                } else {
                    throw MontadorException(MontadorException::OPERACAO_INVALIDA);
                }
            }
        } catch (MontadorException &e) {
            errors.adicionaError(e.error, linha, contadorLinha);
            continue;
        }
    }

    if (errors.contemErrors()) {
        throw PassagemException("Segunda Passagem", &errors);
    }

    return code;
}

#endif