#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *arquivo;
    int buffer[100]; // buffer para armazenar dados lidos
    size_t elementosLidos;

    arquivo = fopen("saida.bin", "rb");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    
    // Le os dados do arquivo binário para o buffer
    elementosLidos = fread(buffer, sizeof(int), 100, arquivo);

    // Exibe os dados lidos no console
    for (size_t i = 0; i < elementosLidos; i++)
    {
        printf("%d ", buffer[i]);
    }

    printf("\n");

    fclose(arquivo);
    
    return 0;
}