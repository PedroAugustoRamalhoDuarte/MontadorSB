#ifndef UTILS
#define UTILS

#include <string>
#include <regex>

using namespace std;

struct Linha {
    string rotulo;
    string operacao;
    string op1;
    string op2;
};

bool isInteger(const std::string & s){
    return std::regex_match(s, std::regex("-?[0-9]{0,10}"));
}

string toUpperCase(string str) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}

string retiraComentarios(string str) {
    return str;
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

#endif