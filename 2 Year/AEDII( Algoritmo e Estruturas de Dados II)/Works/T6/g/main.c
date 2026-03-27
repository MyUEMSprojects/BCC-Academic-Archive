#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Necessário para strcmp

int main(int argc, char* argv[])
{
    if (argc < 4) { // Verifica se os argumentos suficientes foram fornecidos
        fprintf(stderr, "Uso: %s <opcao> <arquivo de leitura> <arquivo de escrita>\n", argv[0]);
        return 1;
    }

    char* opcao = argv[1]; // Corrigido o índice para 1, pois argv[0] é o nome do programa
    char* readName = argv[2];
    char* writeName = argv[3];

    // Verifica se opcao é "c" ou "C"
    if (strcmp(opcao, "c") == 0 || strcmp(opcao, "C") == 0) {
        printf("\nOpção selecionada: %s\n", opcao);
    }
    // Verifica se opcao é "d" ou "D"
    else if (strcmp(opcao, "d") == 0 || strcmp(opcao, "D") == 0) {
        printf("\nOpção selecionada: %s\n", opcao);
    }
    else {
        printf("\nOpção inválida: %s\n", opcao);
    }

    printf("\nArquivo de leitura: %s\n", readName);
    printf("\nArquivo de escrita: %s\n", writeName);

    return 0;
}
