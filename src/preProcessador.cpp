#ifndef PRE_PROCESSADOR
#define PRE_PROCESSADOR

#include "../include/preProcessador.hpp"

using namespace std;


PreProcessador::PreProcessador(const string &nomeArquivo, bool isToWrite) {
    arquivo = new ArquivoFisico(nomeArquivo.c_str());
    if (isToWrite) {
        arquivoPreProcessado = new ArquivoFisico(trocarTipo(nomeArquivo, ".pre").c_str(), true);
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
        throw MontadorException(MontadorException::SECAO_INVALIDA);
    }
}

void PreProcessador::analisaSecao(const string &operacao, const string &op1) {
    if (operacao != "SECTION") {
        switch (secao) {
            case NENHUMA:
                if (operacao != "EQU") {
                    throw MontadorException(MontadorException::SECAO_ERRADA);
                }
                break;
            case TEXT:
                if (operacao == "EQU" or operacao == "SPACE" or operacao == "CONST") {
                    throw MontadorException(MontadorException::SECAO_ERRADA);
                };
                break;
            case DATA:
                if (operacao != "SPACE" and operacao != "CONST") {
                    throw MontadorException(MontadorException::SECAO_ERRADA);
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
    int contadorLinha = 0;
    while (!arquivo->hasEnd()) {
        try {

            arquivo->getLine(&linha);
            contadorLinha++;
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
                        throw MontadorException(MontadorException::DOIS_ROTULOS);
                    }
                }
                ultimoRotulo = "";
            }

            analisaSecao(l.operacao, l.op1);

            if (l.operacao == "EQU") {
                tabelaDeDefinicoes[l.rotulo] = l.op1;
            } else if (l.operacao == "IF") {
                if (tabelaDeDefinicoes.end() != tabelaDeDefinicoes.find(l.op1)) {
                    printLine = !(tabelaDeDefinicoes[l.op1] == "0");
                } else {
                    throw MontadorException(MontadorException::ROTULO_AUSENTE);
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
        } catch (MontadorException &e) {
            errors.adicionaError(e.error, linha, contadorLinha);
            continue;
        }
    }
    arquivo->arquivo.close();
    arquivoPreProcessado->finishWrite();
}

#endif
