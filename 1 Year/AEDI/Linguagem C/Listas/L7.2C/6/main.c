/*
Faça um procedimento que receba 200 números e imprima o maior deles. 
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int numeros[200];
    int maiorNumero();
    int maior = 0;
    for (int i = 0; i <= 200; i++)
    {
        scanf("%d", &numeros[i]);
    }

    maior = maiorNumero(200, numeros);

    printf("\nMAIOR: %d\n", maior);

    system("pause");
    return 0;
}
int maiorNumero(int n, int *numeros)
{
    int maior = 0;

    for (int i = 0; i <= n; i++)
    {
        if (numeros[i] >= maior)
        {
            maior = numeros[i];
        }
    }

    return maior;
}