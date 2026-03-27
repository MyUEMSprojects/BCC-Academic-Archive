#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    void lerVetor(int vetor[], int tam);
    int teste[10];
    int tam = 0;
    int teste2[tam];

    for (int i = 0; i < 10; ++i)
    {
        scanf("%d", &teste[i]);
        if (teste[i] % 2 == 0)
        {
            teste2[tam] = teste[i];
            ++tam;
        }
    }

    lerVetor(teste2, tam);

    system("pause");
    return 0;
}
void lerVetor(int vetor[], int tam)
{
    for (int i = 0; i < tam; ++i)
    {
        printf("vetor[%i]:%d", i + 1, vetor[i]);
    }
}