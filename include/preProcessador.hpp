#ifndef PRE_PROCESSADOR_H
#define PRE_PROCESSADOR_H

#include <string>
#include <map>
#include "ArquivoHandler.hpp"
#include "error.hpp"
#include "utils.hpp"

using namespace std;

class PreProcessador {
    enum SECAO {
        NENHUMA,
        TEXT,
        DATA
    } secao;
public:
    map<string, string> tabelaDeDefinicoes = {};
    ArquivoFisico *arquivo;
    ArquivoHandler *arquivoPreProcessado;

    explicit PreProcessador(const string &nomeArquivo, bool isToWrite);

    static bool isSecaoValida(const string &s);

    void setSecao(const string &s);

    void analisaSecao(const string &operacao, const string &op1, const string &linha, int numLinha);

    void run();
};

#endif