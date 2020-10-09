#ifndef MONTADOR_ERROR_H
#define MONTADOR_ERROR_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class MontadorException : public std::exception {
public:
    enum ERROR_CODE {
        ROTULO_AUSENTE,
        ROTULO_REPETIDO,
        OPERACAO_INVALIDA,
        SECAO_ERRADA,
        QUANTIDADE_OPERANDO,
        OPERANDO_INVALIDO,
        TOKEN_INVALIDO,
        DOIS_ROTULOS,
        TEXT_FALTANTE,
        SECAO_INVALIDA,
    } error;

    MontadorException(ERROR_CODE error) : std::exception() {
        this->error = error;
    }

    const char *what() const noexcept override;
};

struct MontadorError {
    MontadorException::ERROR_CODE code;
    string linha;
    int numLinha;
};

class MontadorErrors {
    vector<MontadorError> errors;
public:
    void adicionaError(MontadorException::ERROR_CODE error, string linha, int numLinha);

    static string mensagemError(MontadorException::ERROR_CODE);

    static string errorTipo(MontadorException::ERROR_CODE);
m
    bool contemErrors();

    void printErrors();
};

class PassagemException : public std::exception {
    string passagem;
public:
    MontadorErrors *errors;

    PassagemException(string passagem, MontadorErrors* errors) : std::exception() {
        this->passagem = passagem;
        this->errors = errors;
    }

    const char *what() const noexcept override;
};


#endif