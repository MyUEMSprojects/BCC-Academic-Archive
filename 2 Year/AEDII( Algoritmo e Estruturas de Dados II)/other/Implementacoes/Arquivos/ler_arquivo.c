#include <stdio.h>
#include <stdlib.h>

// leitura de arquivo texto

int main()
{
    FILE *arquivo;
    char linha[100];

    arquivo = fopen("arquivo.txt", "r");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo");
        return 1;
    }

    while(fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        printf("%s", linha);
    }

    fclose(arquivo);

    return 0;
}