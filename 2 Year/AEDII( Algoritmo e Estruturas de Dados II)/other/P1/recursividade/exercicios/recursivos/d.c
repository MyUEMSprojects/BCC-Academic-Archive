#include <stdio.h>

int verificar(int *s, int x, int inicio, int fim)
{
    if(inicio > fim)
    {
        return 0;
    }

    if(s[inicio] == x)
    {
        return 1;
    }

    return verificar(s, x, inicio + 1, fim);
}

int main()
{
    int vetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);

    printf("%d", verificar(vetor, 5, 0, tamanho - 1));

    return 0;
}