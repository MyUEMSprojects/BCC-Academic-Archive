#include <stdio.h>
#include <stdlib.h>
#define M 100

void inserir(int* L, int x, int i, int* n);
int busca(int* L, int x, int* n);
void remover(int* L, int x, int* n);
void imprimir(int* L, int* n);


int main()
{

    // aloca um array com "memória" de tamanho Ma  
    int *L = (int*) malloc(M * sizeof(int));
    int n = 0;


    if(L == NULL)
    {
        printf("\nErro ao alocar Lista Sequencial");
        return -1;
    }

    inserir(L, 1, 0, &n);
    inserir(L, 2, 0, &n);
    imprimir(L, &n);
    printf("\n");
    remover(L, 1, &n);
    imprimir(L, &n);

    free(L);
    return 0;
}

// inserir em uma lista em alocação sequencial
void inserir(int* L, int x, int i, int* n)
{
    if(*n >= M) return;
    if(L[i] == x) return;

    if(L[i] < x && L[i + 1] < x)
    {
        int j = *n;

        while (j > i)
        {
            L[j] = L[j - 1];
            j = j - 1;
        }
        L[i] = x;
        *n = *n + 1;
    }

    if(x > L[i])
    {
        inserir(L, x, i + 1, n);
    }
}

void remover(int* L, int x, int* n)
{
    int i = busca(L, x, n);

    if (i == 0) return;
    int j = *n;

    while (i < j)
    {
        L[i] = L[i + 1];
        i++;
    }

    *n = *n - 1;
}

int busca(int* L, int x, int* n)
{
    int i = 0;

    while (L[i] != x && i < *n)
    {
        i++;
    }

    if(i < *n)
    {
        return i;
    }
    else
    {
        return 0;
    }
    
}

void imprimir(int* L, int* n)
{
    for (int i = 0; i < *n; i++)
    {
        printf("\n%d", L[i]);       
    }
}