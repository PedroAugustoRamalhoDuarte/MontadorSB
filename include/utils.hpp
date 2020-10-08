#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <regex>
#include "ArquivoHandler.hpp"

using namespace std;

struct Linha {
    string rotulo;
    string operacao;
    string op1;
    string op2;
};

bool isInteger(const std::string & s);

string toUpperCase(string str);

bool somenteRotulo(const Linha& linha);

string trocarTipo(string nome, const string& terminacao);

Linha coletaTermosDaLinha(const string &linha);

string linhaToString(const Linha& linha);

void gerarArquivoObjeto(string codigo, string filename);

#endif