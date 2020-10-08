#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <regex>

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

Linha coletaTermosDaLinha(const string &linha);

string linhaToString(const Linha& linha);

#endif