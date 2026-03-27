#include <stdio.h>

int main()
{
    FILE *fp;
    int data[] = {10, 20, 30, 40, 50}; // Exemplo de dados
    int n = sizeof(data) / sizeof(data[0]);

    // Abre o arquivo binário para escrita
    fp = fopen("arquivo.bin", "wb");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Escreve os dados no arquivo binário
    fwrite(data, sizeof(int), n, fp);

    // Fecha o arquivo
    fclose(fp);

    printf("Arquivo binário 'arquivo.bin' gerado com sucesso.\n");
    return 0;
}