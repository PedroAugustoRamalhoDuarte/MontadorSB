#ifndef MONTADOR_ERROR
#define MONTADOR_ERROR

#include <string>

using namespace std;

class MontadorErro : public std::exception {
public:
    string menssagem;

    MontadorErro(const string& msg, const string& tipo, const string& linha, int numLinha): std::exception() {
        this->menssagem.append("Erro(");
        this->menssagem.append(to_string(numLinha));
        this->menssagem.append("): ");
        this->menssagem.append(linha);
        this->menssagem.append("\n");
        this->menssagem.append(tipo);
        this->menssagem.append(": ");
        this->menssagem.append(msg);
    }

    const char * what() const noexcept override;
};



const char *MontadorErro::what() const noexcept {
    return this->menssagem.c_str();
}

#endif