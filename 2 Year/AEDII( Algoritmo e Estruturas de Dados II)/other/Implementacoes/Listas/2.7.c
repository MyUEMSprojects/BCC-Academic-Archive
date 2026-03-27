#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M 100

int busca(int* L, int x, int indice, int* n);
void inserirL1(int *L1, int x, int *n);
void inserirL2(int *L2, int x, int *n, int* tam);
void removerL1(int *L1, int x, int *n);
void removerL2(int *L2, int x, int *n, int* tam);
void imprimir(int *L, int indice, int n);

int main()
{
    // Aloca um array com "memória" de tamanho M
    int *L = (int *)malloc(M * sizeof(int));
    int n = 0, tam = 0;

    if (L == NULL)
    {
        printf("\nErro ao alocar Lista Sequencial");
        return -1;
    }

    inserirL1(L, 1, &n);
    inserirL1(L, 2, &n);
    printf("\nL1: ");
    imprimir(L, 0, n);

    int n2 = M/2;
    printf("\nL2: ");
    inserirL2(L, 1, &n2, &tam);
    inserirL2(L, 2  , &n2, &tam);
    imprimir(L, M/2, n2);
    printf("\nL1 depois da remocao: ");
    removerL1(L, 2, &n);
    imprimir(L, 0, n);
    printf("\nL2 depois da remoca: ");
    removerL2(L, 2, &n2, &tam);
    imprimir(L, M/2, n2);

    // Libera a memória alocada
    free(L);

    return 0;
}

int busca(int *L, int x, int indice, int *n)
{
    int i = indice;

    while (i < *n && L[i] != x)
    {
        i++;
    }

    if(i == *n)
    {
        return -1;
    }
    
    return i;
}

void inserirL1(int *L1, int x, int *n)
{
    if(*n >= M / 2)
    {
        return;
    }

    L1[*n] = x;
    (*n)++;
}

void inserirL2(int *L2, int x, int *n, int* tam)
{
    if(*n >= M)
    {
        return;
    }

    L2[*n] = x;
    (*tam)++;
    (*n)++;
}

void removerL1(int *L1, int x, int *n)
{
    if(*n <= 0)
    {
        return;
    }

    int indice = busca(L1, x, 0, n);
    if(indice != -1)
    {
        for (int i = indice; i < *n-1; i++)
        {
            L1[i] = L1[i+1];
        }
        
        (*n)--;
    }
    else
    {
        return;
    }
}

void removerL2(int *L2, int x, int *n, int* tam)
{
    if(*tam <= 0)
    {
        return;  
    }
    int indice = busca(L2, x, M/2, n);
    if(indice != -1)
    {
        for (int i = indice; i < *n-1; i++)
        {
            L2[i] = L2[i+1];
        }
        
        (*tam)--;
        (*n)--;
    }
    else
    {
        return;
    }
}

void imprimir(int *L, int indice, int n)
{
    for (int i = indice; i < n; i++)
    {
        printf("%d ", L[i]);
    }
    printf("\n");
}