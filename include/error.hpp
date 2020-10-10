#ifndef MONTADOR_ERROR_H
#define MONTADOR_ERROR_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class MontadorException : public std::exception {
public:
    enum ERROR_CODE {
        ROTULO_AUSENTE, // Semantico
        ROTULO_REPETIDO, // Semantico
        SECAO_ERRADA, // Semantico
        OPERACAO_INVALIDA, // Sintatico
        QUANTIDADE_OPERANDO, // Sintatico
        OPERANDO_INVALIDO, // Sintatico
        DOIS_ROTULOS, // Sintatico
        TEXT_FALTANTE, // Sintatico
        SECAO_INVALIDA, // Sintatico
        TOKEN_INVALIDO, // lexico
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

    string mensagemTodosErros();


    bool contemErrors();

    void printErrors();
};

class PassagemException : public std::exception {
    string passagem;
    string mensagem;
public:
    PassagemException(string passagem, string mensagem) : std::exception() {
        this->passagem = passagem;
        this->mensagem = mensagem;
    }

    string mensagemCompleta();

    const char *what() const noexcept override;
};


#endif