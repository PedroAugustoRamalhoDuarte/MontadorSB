#ifndef UTILS
#define UTILS

#include "../include/utils.hpp"

using namespace std;

bool isInteger(const std::string &s) {
    return std::regex_match(s, std::regex("-?[0-9]{0,10}"));
}

string toUpperCase(string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

bool somenteRotulo(const Linha& linha) {
    return !linha.rotulo.empty() and linha.operacao.empty() and linha.op1.empty();
}

Linha coletaTermosDaLinha(const string &linha) {
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

string linhaToString(const Linha &linha) {
    string str;
    if (!linha.rotulo.empty()) {
        str += linha.rotulo + ": ";
    }
    if (!linha.op1.empty()) {
        str += linha.operacao + ' ';
    } else {
        str += linha.operacao;
    }
    if (!linha.op1.empty() and !linha.op2.empty()) {
        str += linha.op1 + ", " + linha.op2;
    } else if (!linha.op1.empty()) {
        str += linha.op1;
    }
    return str;
}


#endif