# MontadorSB
Repositório para o Trabalho 1 de Software Básico 2020/01, que consiste em fazer um montador para um assembly inventado


# Checklist
- NAO ser sensível ao caso, podendo aceitar instruções/diretivas/rótulos em maiúsculas
e minúsculas. (#001) OK
- Desconsiderar tabulações, quebras de linhas e espaços desnecessários em qualquer
lugar do código. (#002) OK
- A diretiva CONST deve aceitar números positivos e negativos (somente em decimal). (#003) OK
- Capacidade de aceitar comentários indicados pelo símbolo “;” em qualquer lugar
do código (#004) OK
- O comando COPY deve utilizar uma vírgula e um espaço entre os operandos
(COPY A, B) (#005) OK
- Poder criar um rótulo, dar quebra de linha e continuar a linha depois (o rótulo
seria equivalente a linha seguinte) (#006)



# Erros do montador:
- declarações e rótulos ausentes;
- declarações e rótulos repetidos OK;
- diretivas inválidas; OK
- instruções inválidas; OK
- diretivas ou instruções na seção errada OK;
- instruções com a quantidade de operando errado; OK
- instruções com o tipo de operando inválido;
- tokens inválidos; OK
- dois rótulos na mesma linha;
- seção TEXT faltante;
- seção inválida; OK