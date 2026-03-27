#include <stdio.h>

int maior_numero(int *s, int maior ,int inicio, int fim)
{
    if(inicio > fim)
    {
        return maior;
    }

    if(s[inicio] >= maior)
    {
        return maior_numero(s, s[inicio], inicio + 1, fim);
    }
}

int main()
{
    int vetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);

    printf("%d", maior_numero(vetor, -2147483648, 0, tamanho - 1));

    return 0;
}