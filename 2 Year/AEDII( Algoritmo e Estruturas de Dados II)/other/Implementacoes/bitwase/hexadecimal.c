#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void manipulacao_bits_haxa()
{
    char caractere = 'A'; // Caractere cujo valor na tabela ASCII é 0x41 // 01000001 = 65
    unsigned int valor_hex = caractere; // Armazena o valor hexadecimal do caractere

    printf("Caractere: %c\n", caractere);
    printf("Valor hexadecimal: 0x%X\n", valor_hex);

    // Exemplo de operação AND para zerar o quarto bit (a contagem começa do 0)
    unsigned int resultado_and = valor_hex & ~(1 << 4);
    printf("Resultado apoś AND: 0x%X\n", resultado_and);

    // Exemplo de operação OR para setar o segundo bit
    unsigned int resultado_or = valor_hex | (1 << 2); // 0x41(0100) | 0100

    // Exemplo de operação XOR para inverter o quinto bit
    unsigned int resultado_xor = valor_hex ^ (1 << 5); // 0010 0000
    printf("Resultado após XOR: 0x%X\n", resultado_xor);
}

int main()
{
    manipulacao_bits_haxa();
    return 0;
}