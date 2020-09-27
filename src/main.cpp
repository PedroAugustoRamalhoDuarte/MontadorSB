#include "montador.cpp"

int main(){
    try {
        throw MontadorErro("Erro", "Tipo", "LINHA: 123 123", 3);
    } catch (const MontadorErro & ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
};