#ifndef MONTADOR_ERROR
#define MONTADOR_ERROR

#include "../include/error.hpp"

using namespace std;

const char *MontadorErro::what() const noexcept {
    return this->menssagem.c_str();
}

#endif