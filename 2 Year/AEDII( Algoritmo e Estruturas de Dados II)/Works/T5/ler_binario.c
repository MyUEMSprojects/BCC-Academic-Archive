#include <stdio.h>   /* default */
#include <stdlib.h>  /* malloc */
#include <stdint.h>  /* int32_t */
#include <unistd.h>  /* sleep  */
#include <stdbool.h> /* bool */
#include "math.h"    /* floor */
#include <time.h>
#include <assert.h>

size_t contar_numeros(const char *nome_arquivo)
{
    FILE *fp;
    size_t tamanho_arquivo;
    size_t quantidade_numeros;

    // Abre o arquivo binário para leitura
    fp = fopen(nome_arquivo, "rb");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }

    // Move o ponteiro do arquivo para o final
    fseek(fp, 0, SEEK_END);

    // Obtém o tamanho do arquivo em bytes
    tamanho_arquivo = ftell(fp);

    // Calcula o número de inteiros de 32 bits no arquivo
    quantidade_numeros = tamanho_arquivo / sizeof(int32_t);

    // Fecha o arquivo
    fclose(fp);

    return quantidade_numeros;
}

int32_t *ler_sequencia(const char *nome_arquivo, int N)
{
    FILE *fp;
    int32_t *V = NULL;

    // Aloca memória para o vetor de inteiros
    assert((V = malloc(N * sizeof(int32_t))) != NULL);

    // Abre o arquivo binário para leitura
    assert((fp = fopen(nome_arquivo, "rb")) != NULL);

    // Lê N inteiros do arquivo e armazena no vetor
    fread(V, sizeof(int32_t), N, fp);

    // Fecha o arquivo
    fclose(fp);

    // Retorna o vetor
    return V;
}

void imprimir(int32_t *V, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("V[%i]:%d\n", i, V[i]);
    }
}

int main(int argc, char* argv[])
{
    if(argc > 2)
    {
        printf("use: <nome_arquivo>");
        return -1;
    }

    char *nome_arquivo = argv[1];
    int N = (int)contar_numeros(nome_arquivo);

    // Chama a função para ler a sequência do arquivo binário
    int32_t *sequencia = ler_sequencia(nome_arquivo, N);

    imprimir(sequencia, N);
    return 0;
}