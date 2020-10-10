### Nome: Pedro Augusto Ramalho Duarte
### Matrícula: 17/0163717
### Escolha: Implementação de Erros

# MontadorSB
Repositório para o Trabalho 1 de Software Básico 2020/01, que consiste em fazer um montador para um assembly inventado.

# Como rodar?
- Digite make para buildar o projeto;
- E parar executar ./montador nomedoarquivo;
- Caso coloque o argumento -p, o arquivo só será processado e a saída será nomedoarquivo.pre, 
- Caso coloque o argumento -o, o arquivo só será  montado e a saída será nomedoarquivo.obj 
- Caso não coloque nenhum argumento será pré-processado em memória e montado com a saída nomedoarquivo.obj

# Checklist
As seguintes funcionalidades podem ser procuradas no código, pesquisando pelo ID da funcionalidade
- NAO ser sensível ao caso, podendo aceitar instruções/diretivas/rótulos em maiúsculas
e minúsculas. (#001)
- Desconsiderar tabulações, quebras de linhas e espaços desnecessários em qualquer
lugar do código. (#002)
- A diretiva CONST deve aceitar números positivos e negativos (somente em decimal). (#003)
- Capacidade de aceitar comentários indicados pelo símbolo “;” em qualquer lugar
do código (#004)
- O comando COPY deve utilizar uma vírgula e um espaço entre os operandos
(COPY A, B) (#005)
- Poder criar um rótulo, dar quebra de linha e continuar a linha depois (o rótulo
seria equivalente a linha seguinte) (#006)


# Erros do montador:
O montador possui os seguintes erros que podem, ser detalhamente observados na classe MontadorException em include/error.hpp
- declarações e rótulos ausentes;
- declarações e rótulos repetidos;
- diretivas inválidas;
- instruções inválidas;
- diretivas ou instruções na seção errada;
- instruções com a quantidade de operando errado;
- instruções com o tipo de operando inválido;
- tokens inválidos;
- dois rótulos na mesma linha;
- seção TEXT faltante;
- seção inválida;