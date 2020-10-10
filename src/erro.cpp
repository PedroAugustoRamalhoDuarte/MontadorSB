#ifndef MONTADOR_ERROR
#define MONTADOR_ERROR


#include "../include/error.hpp"

using namespace std;

const char *MontadorException::what() const noexcept {
    return (const char *) this->error;
}

void MontadorErrors::adicionaError(MontadorException::ERROR_CODE error, string linha, int numLinha) {
    errors.push_back((MontadorError) {error, linha, numLinha});
}

string MontadorErrors::mensagemError(MontadorException::ERROR_CODE error) {
    switch (error) {
        case MontadorException::ROTULO_AUSENTE:
            return "Declaração ou Rótulo Ausente";
        case MontadorException::ROTULO_REPETIDO:
            return "Declarao ou Rótulo Repetido";
        case MontadorException::OPERACAO_INVALIDA:
            return "Diretiva ou Instrução inválida";
        case MontadorException::SECAO_ERRADA:
            return "Diretivas ou instruções na seção errada";
        case MontadorException::QUANTIDADE_OPERANDO:
            return "Instruções com a quantidade de operando errado";
        case MontadorException::OPERANDO_INVALIDO:
            return "Instruções com o tipo de operando inválido";
        case MontadorException::TOKEN_INVALIDO:
            return "Tokens inválidos";
        case MontadorException::DOIS_ROTULOS:
            return "Dois rótulos na mesma linha";
        case MontadorException::TEXT_FALTANTE:
            return "Seção TEXT faltante";
        case MontadorException::SECAO_INVALIDA:
            return "Seção inválida";
    }
}

string MontadorErrors::errorTipo(MontadorException::ERROR_CODE error) {
    if (error < MontadorException::SECAO_ERRADA) {
        return "Erro Semântico";
    } else if (error == MontadorException::TOKEN_INVALIDO) {
        return "Erro Léxico";
    } else {
        return "Erro Sintático";
    }
}

bool MontadorErrors::contemErrors() {
    return !errors.empty();
}

string MontadorErrors::mensagemTodosErros() {
    string errorMessage;
    for (const auto &error: errors) {
        errorMessage = errorMessage + errorTipo(error.code) + ": " + mensagemError(error.code) +
        "\n Na linha (" + to_string(error.numLinha) + "): " + error.linha + "\n";
    }
    return errorMessage;
}

void MontadorErrors::printErrors() {
    for (const auto &error: errors) {
        std::cout << errorTipo(error.code) << ": " << mensagemError(error.code) <<
                  std::endl << "Na linha (" << to_string(error.numLinha) << "): " << error.linha << std::endl;
    }
}

const char *PassagemException::what() const noexcept {
    return this->passagem.c_str();
}

string PassagemException::mensagemCompleta() {
    return "Erro na: " + this->passagem + "\n" + this->mensagem;
}

#endif