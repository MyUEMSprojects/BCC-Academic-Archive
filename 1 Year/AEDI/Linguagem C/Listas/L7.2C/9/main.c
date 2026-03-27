#include <stdio.h>
#include <stdlib.h>

int vetorGlobal[];
int tamVetor = 0;

int main(int argc, char const *argv[])
{

    int vetor[5];
    int procedimento();

    for (int i = 0; i <= 5; i++)
    {
        scanf("%d", &vetor[i]);
    }

    procedimento(5, vetor);

    system("pause");
    return 0;
}

int procedimento(int n, int *vetor)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (vetor[i] == vetor[j])
            {
                vetorGlobal[i] = vetor[i];
                tamVetor++;
            }
        }
    }
    for (int i = 0; i < tamVetor; i++)
    {
        printf("%d\n", vetorGlobal[i]);
    }
}