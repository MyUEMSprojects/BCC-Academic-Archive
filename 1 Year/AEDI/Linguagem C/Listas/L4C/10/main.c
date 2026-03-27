#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int tam = 0;

    scanf("%d", &tam);

    int vetor[tam];
    int vetor_par[tam];
    int vetor_impar[tam];

    for (int i = 0; i < tam; i++)
    {
        scanf("%d", &vetor[i]);
    }

    for (int i = 0; i < tam; i++)
    {
        if (vetor[i] % 2 == 0)
        {
            vetor[i] = vetor_par[i];
        }
        if (vetor[i] % 3 == 0)
        {
            vetor[i] = vetor_impar[i];
        }
    }

    for (int i = 0; i < tam; i++)
    {
        printf("\n%d", &vetor_impar[i]);
    }

    printf("------------------");

    for (int i = 0; i < tam; i++)
    {
        printf("%d", &vetor_par[i]);
    }

    system("pause");
    return 0;
}
