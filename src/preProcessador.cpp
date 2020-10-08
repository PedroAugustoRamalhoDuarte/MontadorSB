#ifndef PRE_PROCESSADOR
#define PRE_PROCESSADOR

#include "../include/preProcessador.hpp"

using namespace std;


PreProcessador::PreProcessador(const string &nomeArquivo, bool isToWrite) {
    arquivo = new ArquivoFisico(nomeArquivo.c_str());
    if (isToWrite) {
        // TODO trocar lógica de nome de arquivo
        arquivoPreProcessado = new ArquivoFisico((nomeArquivo + ".out").c_str());
    } else {
        arquivoPreProcessado = new ArquivoEmMemoria;
    }
    secao = NENHUMA;
}

// Métodos para controle de seção
// ----------------------------------------------------------------
bool PreProcessador::isSecaoValida(const string &s) {
    return (s == "TEXT" or s == "DATA");
}

void PreProcessador::setSecao(const string &s) {
    if (isSecaoValida(s)) {
        if (s == "TEXT") {
            secao = TEXT;
        } else if (s == "DATA") {
            secao = DATA;
        }
    } else {
        // TODO MONTAR ESSE ERRO
        throw MontadorErro("Seção Inválida", "TIPO", "linha", 0);
    }
}

void PreProcessador::analisaSecao(const string &operacao, const string &op1, const string &linha, int numLinha) {
    if (operacao != "SECTION") {
        switch (secao) {
            case NENHUMA:
                if (operacao != "EQU") {
                    throw MontadorErro("Operação fora da seção adequada", "TIPO", linha, numLinha);
                }
                break;
            case TEXT:
                if (operacao == "EQU" or operacao == "SPACE" or operacao == "CONST") {
                    throw MontadorErro("Operação fora da seção adequada", "TIPO", linha, numLinha);
                };
                break;
            case DATA:
                if (operacao != "SPACE" and operacao != "CONST") {
                    throw MontadorErro("Operação fora da seção adequada", "TIPO", linha, numLinha);
                };
                break;
        }
    } else {
        setSecao(op1);
    }
}
// ----------------------------------------------------------------

void PreProcessador::run() {
    string linha, ultimoRotulo;
    bool printLine = true;
    secao = NENHUMA;
    int contadorLinha = 1;

    while (!arquivo->hasEnd()) {
        arquivo->getLine(&linha);

        Linha l = coletaTermosDaLinha(linha);

        // Somente rótulo na linha #006
        if (somenteRotulo(l)) {
            ultimoRotulo = l.rotulo;
            continue;
        } else {
            if (!ultimoRotulo.empty()) {
                if (l.rotulo.empty()) {
                    l.rotulo = ultimoRotulo;
                } else {
                    throw MontadorErro("Dois rótulos na mesma linha", "TIPO", linha, contadorLinha);
                }
            }
            ultimoRotulo = "";
        }

        analisaSecao(l.operacao, l.op1, linha, contadorLinha);

        if (l.operacao == "EQU") {
            tabelaDeDefinicoes[l.rotulo] = l.op1;
        } else if (l.operacao == "IF") {
            if (tabelaDeDefinicoes.end() != tabelaDeDefinicoes.find(l.op1)) {
                printLine = !(tabelaDeDefinicoes[l.op1] == "0");
            } else {
                throw MontadorErro("Definição não encontrada", "TIPO", linha, contadorLinha);
            }
        } else {
            if (tabelaDeDefinicoes.end() != tabelaDeDefinicoes.find(l.op1)) {
                l.op1 = tabelaDeDefinicoes[l.op1];
            }
            if (tabelaDeDefinicoes.end() != tabelaDeDefinicoes.find(l.op2)) {
                l.op2 = tabelaDeDefinicoes[l.op2];
            }
            if (printLine) {
                arquivoPreProcessado->writeLine(linhaToString(l));
            } else {
                printLine = true;
            }
        }

        contadorLinha += 1;
    }
    arquivo->arquivo.close();
    arquivoPreProcessado->finishWrite();
}

#endif
