#include <string>
#include <map>
#include "ArquivoHandler.cpp"
#include "erro.cpp"
#include "utils.cpp"

using namespace std;

class PreProcessador{
    enum SECAO {
        NENHUMA,
        TEXT,
        DATA
    } secao;

    map<string, string> tabelaDeDefinicoes = {};
    ArquivoFisico *arquivo;
    ArquivoHandler *arquivoPreProcessado;

    public:
    explicit PreProcessador(const char *nomeArquivo, bool isToWrite) {
        arquivo = new ArquivoFisico(nomeArquivo);
        if (isToWrite) {
            arquivoPreProcessado = new ArquivoFisico(nomeArquivo);
        } else {
            arquivoPreProcessado = new ArquivoEmMemoria;
        }
        secao = NENHUMA;
    }

    // Métodos para controle de seção
    static bool isSecaoValida(const string& s) {
        return (s == "TEXT" or s == "DATA");
    }

    void setSecao(const string& s) {
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

    void analisaSecao(const string &operacao, const string& op1, const string &linha, int numLinha) {
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

    void run() {
        string linha;
        bool isInsideValid = true;
        secao = NENHUMA;
        int contador_linha = 1;

        while (!arquivo->hasEnd()) {
            arquivo->getLine(&linha);

            Linha l = coletaTermosDaLinha(linha);

            analisaSecao(l.operacao, l.op1, linha, contador_linha);

            if (l.operacao == "EQU") {
                tabelaDeDefinicoes[l.rotulo] = l.op1;
            } else if (l.operacao == "IF") {
                if (tabelaDeDefinicoes.end() != tabelaDeDefinicoes.find(l.op1)) {
                    isInsideValid = !(tabelaDeDefinicoes[l.op1] == "0");
                } else {
                    throw MontadorErro("Definição não encontrada", "TIPO", linha, contador_linha);
                }
            } else {
                if (isInsideValid) {
                    arquivoPreProcessado->writeLine(linha);
                }
            }

            contador_linha += 1;
        }
        arquivo->arquivo.close();
    }
};