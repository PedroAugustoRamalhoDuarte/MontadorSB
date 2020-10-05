#include <iostream>
#include "montador.cpp"
#include "preProcessador.cpp"

int main(int argc, char **argv) {
    std::cout << argc << std::endl;
    if (argc == 2) {
        // ex ./montador myprogram.asm
        std::cout << "Montado programa: " << argv[1] << endl;
        PreProcessador preProcessador(argv[1], false);
        preProcessador.run();
        Montador montador(preProcessador.arquivoPreProcessado);
        montador.primeiraPassagem();
        std::cout << montador.segundaPassagem() << endl;
    } else if (argc == 3) {
        if (strcmp(argv[1], "-p") == 0) {
            std::cout << "Pré-processando: " << argv[2] << endl;
            // ex: ./montador -p myprogram.asm
            PreProcessador preProcessador(argv[2], true);
            preProcessador.run();
        } else if (strcmp(argv[1], "-c") == 0) {
            std::cout << "Montando: " << argv[2] << endl;
            // ex: /montador -c myprogram.pre
            auto *arquivoFisico = new ArquivoFisico(argv[2]);
            Montador montador(arquivoFisico);
            montador.primeiraPassagem();
            std::cout << montador.segundaPassagem() << endl;
        } else {
            throw invalid_argument("Argumento inválido, utilize -p, -c ou nada");
        }
    }
    return 0;
};