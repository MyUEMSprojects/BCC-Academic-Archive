#include <stdio.h>   /* default */
#include <stdlib.h>  /* malloc */
#include <stdint.h>  /* int32_t */
#include <unistd.h>  /* sleep  */
#include <stdbool.h> /* bool */
#include "math.h"    /* floor */
#include <sys/time.h> /* gettimeofday */
#include <assert.h>  /* FILE */

void swap(int32_t * a, int32_t *b);

void bubble_sort_original(int32_t *V, int n);
void bubble_sort_melhorado(int32_t *V, int n);
void insertion_sort(int32_t *V, int n);

void intercalar(int32_t *V, int32_t *temp, int ini1, int ini2, int fim2);
void merge_sort(int32_t *V, int esq, int dir, int32_t *temp);
void merge_sort_iniciar(int32_t *V, int n);

int PIVO(int32_t *V, int ini, int fim);
void quick_sort_mediana_tres(int32_t *V, int ini, int fim);

int PIVO_ALEATORIO(int32_t *V, int ini, int fim);
void quick_sort_aleatorio(int32_t *V, int ini, int fim);

void quick_sort_ultimo_elemento(int32_t *V, int ini, int fim);

void descer(int32_t *V, int i, int n);
void arranjar(int32_t *V, int n);
void heap_sort(int32_t *V, int n);

size_t contar_numeros(const char *nome_arquivo);
int32_t *ler_sequencia(const char *nome_arquivo, int N);
void salvar_sequencia(const char *nome_arquivo, int32_t *vetor, int N);

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("use: <numero_do_metodo> <nome_arquivo_entrada> <nome_arquivo_saida> \n");
        return -1;
    }

    char *nome_arquivo = argv[2];
    char *nome_arquivo_ordenado = argv[3];

    struct timeval inicio, fim;
    double tempo_execucao;

    int N = (int)contar_numeros(nome_arquivo);
    int32_t *V = ler_sequencia(nome_arquivo, N);

    switch (atoi(argv[1]))
    {
    case 1:
        gettimeofday(&inicio, NULL);
        bubble_sort_original(V, N);
        gettimeofday(&fim, NULL);
        break;
    case 2:
        gettimeofday(&inicio, NULL);
        bubble_sort_melhorado(V, N);
        gettimeofday(&fim, NULL);
        break;
    case 3:
        gettimeofday(&inicio, NULL);
        insertion_sort(V, N);
        gettimeofday(&fim, NULL);
        break;
    case 4:
        gettimeofday(&inicio, NULL);
        merge_sort_iniciar(V, N);
        gettimeofday(&fim, NULL);
        break;
    case 5:
        gettimeofday(&inicio, NULL);
        quick_sort_ultimo_elemento(V, 0, N - 1);
        gettimeofday(&fim, NULL);
        break;
    case 6:
        gettimeofday(&inicio, NULL);
        quick_sort_aleatorio(V, 0, N - 1);
        gettimeofday(&fim, NULL);
        break;
    case 7:
        gettimeofday(&inicio, NULL);
        quick_sort_mediana_tres(V, 0, N - 1);
        gettimeofday(&fim, NULL);
        break;
    case 8:
        gettimeofday(&inicio, NULL);
        heap_sort(V, N);
        gettimeofday(&fim, NULL);
        break;
    default:
        printf("numero do metodo incorreto!\n");
        break;
    }

    tempo_execucao = (fim.tv_sec - inicio.tv_sec) +
                     (fim.tv_usec - inicio.tv_usec) / 1000000.0;

    printf("Tempo de execução: %.3f segundos\n", tempo_execucao);

    salvar_sequencia(nome_arquivo_ordenado, V, N);

    free(V);

    return 0;
}

void swap(int32_t *a, int32_t *b)
{
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort_original(int32_t *V, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (V[j] > V[j + 1])
            {
                int32_t aux = V[j];
                V[j] = V[j + 1];
                V[j + 1] = aux;
            }
        }
    }
}

void bubble_sort_melhorado(int32_t *V, int n)
{
    bool mudou = true;
    int n_linha = n, guarda = n;

    while (mudou)
    {
        int j = 0;
        mudou = false;

        while (j < n_linha - 1)
        {
            if (V[j] > V[j + 1])
            {
                int32_t aux = V[j];
                V[j] = V[j + 1];
                V[j + 1] = aux;

                mudou = true;
                guarda = j + 1;
            }

            j = j + 1;
        }

        n_linha = guarda;
    }
}

void insertion_sort(int32_t *V, int n)
{
    for (int i = 1; i < n; i++)
    {
        int32_t valor = V[i];
        int j = i - 1;

        while (j >= 0 && valor < V[j])
        {
            V[j + 1] = V[j];
            j = j - 1;
        }
        V[j + 1] = valor;
    }
}

void intercalar(int32_t *V, int32_t *temp, int ini1, int ini2, int fim2)
{
    int fim1 = ini2 - 1;
    int ind = ini1;

    while (ini1 <= fim1 && ini2 <= fim2)
    {
        if (V[ini1] < V[ini2])
        {
            temp[ind] = V[ini1];
            ini1 = ini1 + 1;
        }
        else
        {
            temp[ind] = V[ini2];
            ini2 = ini2 + 1;
        }
        ind = ind + 1;
    }

    while (ini1 <= fim1)
    {
        temp[ind] = V[ini1];
        ini1 = ini1 + 1;
        ind = ind + 1;
    }

    while (ini2 <= fim2)
    {
        temp[ind] = V[ini2];
        ini2 = ini2 + 1;
        ind = ind + 1;
    }

    for (int i = ini1 - (ind - ini1); i < ind; i++)
    {
        V[i] = temp[i];
    }
}

void merge_sort(int32_t *V, int esq, int dir, int32_t *temp)
{
    if (esq < dir)
    {
        int centro = (esq + dir) / 2;
        merge_sort(V, esq, centro, temp);
        merge_sort(V, centro + 1, dir, temp);

        intercalar(V, temp, esq, centro + 1, dir);
    }
}

void merge_sort_iniciar(int32_t *V, int n)
{
    int32_t *temp = (int32_t *)malloc(n * sizeof(int32_t));

    if (temp == NULL)
    {
        printf("merge_sort_iniciar: Erro na alocação de memória\n");
    }
    else
    {
        merge_sort(V, 0, n - 1, temp);
        free(temp); // Libera a memória alocada uma única vez
    }
}

int PIVO(int32_t *V, int ini, int fim)
{
    int meio = (ini + fim) / 2;

    if (V[ini] > V[meio])
        swap(&V[ini], &V[meio]);
    if (V[ini] > V[fim])
        swap(&V[ini], &V[fim]);
    if (V[meio] > V[fim])
        swap(&V[meio], &V[fim]);

    swap(&V[meio], &V[fim]);
    return fim;
}

void quick_sort_mediana_tres(int32_t *V, int ini, int fim)
{
    if (fim <= ini)
        return;

    int pivo_idx = PIVO(V, ini, fim);
    int32_t pivo = V[pivo_idx];

    int i = ini, j = fim - 1;

    while (i <= j)
    {
        while (V[i] < pivo)
            i++;
        while (V[j] > pivo && j > ini)
            j--;

        if (i <= j)
        {
            swap(&V[i], &V[j]);
            i++;
            j--;
        }
    }

    swap(&V[i], &V[fim]);

    quick_sort_mediana_tres(V, ini, i - 1);
    quick_sort_mediana_tres(V, i + 1, fim);
}

int PIVO_ALEATORIO(int32_t *V, int ini, int fim)
{
    int pivo_ind = ini + rand() % (fim - ini + 1);
    swap(&V[pivo_ind], &V[fim]);
    return fim;
}

void quick_sort_aleatorio(int32_t *V, int ini, int fim)
{
    if (fim <= ini)
        return;

    int pivo_ind = PIVO_ALEATORIO(V, ini, fim);
    int32_t pivo = V[pivo_ind];

    int i = ini, j = fim - 1;

    while (i <= j)
    {
        while (V[i] < pivo)
            i++;
        while (V[j] > pivo && j > ini)
            j--;

        if (i <= j)
        {
            swap(&V[i], &V[j]);
            i++;
            j--;
        }
    }

    swap(&V[i], &V[fim]);

    quick_sort_aleatorio(V, ini, i - 1);
    quick_sort_aleatorio(V, i + 1, fim);
}

void quick_sort_ultimo_elemento(int32_t *V, int ini, int fim)
{
    if (fim <= ini)
        return;

    int32_t pivo = V[fim];

    int i = ini, j = fim - 1;

    while (i <= j)
    {
        while (V[i] < pivo)
            i++;
        while (V[j] > pivo && j > ini)
            j--;

        if (i <= j)
        {
            swap(&V[i], &V[j]);
            i++;
            j--;
        }
    }

    swap(&V[i], &V[fim]);

    quick_sort_ultimo_elemento(V, ini, i - 1);
    quick_sort_ultimo_elemento(V, i + 1, fim);
}

void descer(int32_t *V, int i, int n)
{
    int j = 2 * i + 1;

    while (j <= n)
    {
        if (j < n && V[j + 1] > V[j])
        {
            j++;
        }

        if (V[i] < V[j])
        {
            swap(&V[i], &V[j]);
            i = j;
            j = 2 * i + 1;
        }
        else
        {
            break;
        }
    }
}

void arranjar(int32_t *V, int n)
{
    for (int i = (n / 2) - 1; i >= 0; i--)
    {
        descer(V, i, n - 1);
    }
}

void heap_sort(int32_t *V, int n)
{
    arranjar(V, n);
    int m = n - 1;

    while (m > 0)
    {
        swap(&V[0], &V[m]);
        m--;
        descer(V, 0, m);
    }
}

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

void salvar_sequencia(const char *nome_arquivo, int32_t *vetor, int N)
{
    FILE *fp;

    // Abre o arquivo binário para escrita
    assert((fp = fopen(nome_arquivo, "wb")) != NULL);

    // Escreve N inteiros no arquivo
    fwrite(vetor, sizeof(int32_t), N, fp);

    // Fecha o arquivo
    fclose(fp);
}