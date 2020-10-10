#include <iostream>
#include "../include/montador.hpp"
#include "../include/preProcessador.hpp"
#include "../include/utils.hpp"

int main(int argc, char **argv) {
    try {
        if (argc == 2) {
            // ex ./montador myprogram.asm
            std::cout << "Montando programa: " << argv[1] << endl;
            PreProcessador preProcessador(argv[1], false);
            preProcessador.run();
            Montador montador(preProcessador.arquivoPreProcessado);
            montador.primeiraPassagem();
            gerarArquivoObjeto(montador.segundaPassagem(), argv[1]);
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
                gerarArquivoObjeto(montador.segundaPassagem(), argv[2]);
            } else {
                throw invalid_argument("Argumento inválido, utilize -p, -c ou nada");
            }
        }
    } catch (PassagemException &e) {
        std::cout << e.mensagemCompleta() << std::endl;
    }
    return 0;
};