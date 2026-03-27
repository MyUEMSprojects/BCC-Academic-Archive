#include <stdio.h>
#include <stdlib.h>

// só imprimir, as funções ja estao funcionando

int main(void)
{
    void lerNegativo(int vetor[], int n);
    void Multiplo(int vetor[], int n, int number);

    int n1;
    int vetorA[n1];
    int number;

    printf("Tamanho de vetor:\n");
    scanf("%d", &n1);

    printf("Numero:");
    scanf("%d", &number);

    for (int i = 0; i < n1; ++i)
    {
        scanf("%d", &vetorA[i]);
    }

    lerNegativo(vetorA, n1);
    Multiplo(vetorA, n1, number);

    system("pause");
    return 0;
}
void lerNegativo(int vetor[], int n)
{
    int tam = 0;
    int vetorCopia[tam];
    for (int i = 0; i < n; ++i)
    {
        if (vetor[i] < 0)
        {
            vetorCopia[tam] = vetor[i];
            ++tam;
        }
    }
}

void Multiplo(int vetor[], int n, int number)
{
    int tam = 0;
    int vetorCopia[tam];
    for (int i = 0; i < n; ++i)
    {
        if (vetor[i] % number == 0)
        {
            vetorCopia[tam] = vetor[i];
            ++tam;
        }
    }
}