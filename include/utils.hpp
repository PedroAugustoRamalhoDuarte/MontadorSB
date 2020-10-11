#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <regex>
#include "ArquivoHandler.hpp"
#include "error.hpp"

using namespace std;

struct Linha {
    string rotulo;
    string operacao;
    string op1;
    string op2;
};

bool isInteger(const std::string &s);

bool isVariavelValida(const string &variavel);

string toUpperCase(string str);

bool somenteRotulo(const Linha &linha);

string trocarTipo(string nome, const string &terminacao);

Linha coletaTermosDaLinha(string linha, bool isToThrowErros = true);

string linhaToString(const Linha &linha);

void gerarArquivoObjeto(string codigo, string filename);

std::string &ltrim(std::string &s);

// trim from end
std::string &rtrim(std::string &s);

// trim from both ends
std::string &trim(std::string &s);

#endif