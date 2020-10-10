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
            if (preProcessador.errors.contemErrors()) {
                std::cout << "Erro no pré-processamento:" << std::endl;
                std::cout << preProcessador.errors.mensagemTodosErros() << std::endl;
            }
            Montador montador(preProcessador.arquivoPreProcessado);
            montador.primeiraPassagem();
            gerarArquivoObjeto(montador.segundaPassagem(), argv[1]);
            std::cout << "Foi gerado o arquivo: " << trocarTipo(argv[1], ".obj") << std::endl;
        } else if (argc == 3) {
            if (strcmp(argv[1], "-p") == 0) {
                std::cout << "Pré-processando: " << argv[2] << endl;
                // ex: ./montador -p myprogram.asm
                PreProcessador preProcessador(argv[2], true);
                preProcessador.run();
                if (preProcessador.errors.contemErrors()) {
                    std::cout << "Erro no pré-processamento:" << std::endl;
                    std::cout << preProcessador.errors.mensagemTodosErros() << std::endl;
                }
                std::cout << "Foi gerado o arquivo: " << trocarTipo(argv[2], ".pre") << std::endl;
            } else if (strcmp(argv[1], "-o") == 0) {
                std::cout << "Montando: " << argv[2] << endl;
                // ex: /montador -c myprogram.pre
                auto *arquivoFisico = new ArquivoFisico(argv[2]);
                Montador montador(arquivoFisico);
                montador.primeiraPassagem();
                gerarArquivoObjeto(montador.segundaPassagem(), argv[2]);
                std::cout << "Foi gerado o arquivo: " << trocarTipo(argv[2], ".obj") << std::endl;
            } else {
                throw invalid_argument("Argumento inválido, utilize -p, -o ou nada");
            }
        }
    } catch (PassagemException &e) {
        std::cout << e.mensagemCompleta() << std::endl;
    }
    return 0;
};