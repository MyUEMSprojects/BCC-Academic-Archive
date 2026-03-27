#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 100

void inserir(int* L, int x, int* n, char* E);
void remover(int* L, int* n, char* E);
void imprimir(int *L, int *n);

int main()
{
    // Aloca um array com "memória" de tamanho M
    int *L = (int *)malloc(M * sizeof(int));
    int n = 0;

    if (L == NULL)
    {
        printf("\nErro ao alocar Lista Sequencial");
        return -1;
    }

    inserir(L, 2, &n, "E1");
    inserir(L, 3, &n, "E1");
    inserir(L, 1, &n, "E2");
    // Imprime o array
    imprimir(L, &n);
    printf("\n");
    remover(L, &n, "E1");
    imprimir(L, &n);
    printf("\n");
    remover(L, &n, "E2");
    imprimir(L, &n);
    
    // Libera a memória alocada
    free(L);

    return 0;
}

void inserir(int *L, int x, int *n, char *E)
{
    if(strcmp(E, "E1") == 0)
    {
        int i = 0;
        int j = *n;

        while (j > i)
        {   
            L[j] = L[j-1];
            j--;
        }

        L[i] = x;
        (*n)++;
    }
    else if(strcmp(E, "E2") == 0)
    {
        L[*n] = x;
        (*n)++;
    }
    else
    {
        return;
    }
}

void remover(int* L, int* n, char* E)
{
    if(strcmp(E, "E1") == 0)
    {
        int i = 0;
        while (i < *n)
        {
            L[i] = L[i + 1];
            i++;
        }
        (*n)--;
    }
    else if(strcmp(E, "E2") == 0)
    {
        if(*n > 0)
        {
            (*n)--;
        }
    }
    else
    {
        return;
    }
}

void imprimir(int *L, int *n)
{
    for (int i = 0; i < *n; i++)
    {
        printf("%d ", L[i]);
    }
    printf("\n");
}