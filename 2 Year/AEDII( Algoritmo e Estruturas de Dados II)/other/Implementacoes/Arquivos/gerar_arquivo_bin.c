#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *arquivo;
    int numeros[] = {10, 20, 30, 40, 50};
    size_t elementosGravados;

    // Abre o arquivo em modo binário de escrito ("wb")
    arquivo = fopen("saida.bin", "wb");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    
    // Escreva o array de números inteiros no arquivo binário
    elementosGravados = fwrite(numeros, sizeof(int), 5, arquivo);

    if(elementosGravados != 5)
    {
        printf("Erro ao gravar os dados no arquivo!\n");
    }
    else
    {
        printf("Arquivo binário gravado com sucesso!\n");
    }

    // Fecha o arquivo
    fclose(arquivo);
    return 0;
}