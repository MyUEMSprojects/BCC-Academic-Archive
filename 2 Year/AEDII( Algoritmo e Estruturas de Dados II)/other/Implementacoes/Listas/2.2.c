#include <stdio.h>
#include <stdlib.h>
#define M 100

int busca_ord1(int *L, int x, int *n);
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

    for (int i = 0; i < M - 1; i++)
    {
        L[n] = i;
        n++;
    }

    // Testa a função de busca com o valor 50
    printf("Índice encontrado: %d\n", busca_ord1(L, 50, &n));

    // Imprime o array
    imprimir(L, &n);

    // Libera a memória alocada
    free(L);

    return 0;
}

int busca_ord1(int *L, int x, int *n)
{
    if (L[*n - 1] >= x) // Verifica se o último elemento é maior ou igual a x
    {
        int i = 0;

        // Procura o índice onde L[i] é igual ou maior que x
        while (i < *n && L[i] < x)
        {
            i++;
        }

        // Verifica se encontrou o valor x
        if (i < *n && L[i] == x)
        {
            return i;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
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