#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int tam = 5;
    int vetorA[tam];
    int repetidos();

    for (int i = 0; i <= tam; i++)
    {
        scanf("%d", &vetorA[i]);
    }

    repetidos(tam, vetorA);

    system("pause");
    return 0;
}

int repetidos(int n, int *vetor)
{

    int vetorB[n];
    int tamanho = n;

    for (int i = 0; i <= n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            if (vetor[i] == vetor[j])
            {
                printf("Valor repetido :%i: nos indices %i e %i", vetor[i], i, j);

                vetorB[i] = vetor[i];
                vetor[i] -= vetor[i];
                tamanho--;
            }
            printf("\n");
        }
    }

    for (int i = 0; i <= tamanho; i++)
    {
        printf("%d\n", &vetorB[i]);
    }
}