/*
Faça um procedimento que receba 300 números inteiros e positivos e determine os 5 números 
maiores considerando a inexistência de números repetidos no conjunto. Os números encontrados 
devem ser armazenados em um vetor global. 
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int numeros[300];
    int maiorNumero();
    for (int i = 0; i <= 300; i++)
    {
        scanf("%d", &numeros[i]);
    }

    maiorNumero(300, numeros);

    system("pause");
    return 0;
}

int maiorNumero(int n, int *numeros)
{
    int primeiro_maior = 0;
    int segundo_maior = 0;
    int terceiro_maior = 0;
    int quarto_maior = 0;
    int quinto_maior = 0;

    for (int i = 0; i <= n; i++)
    {
        if (numeros[i] > primeiro_maior)
        {
            primeiro_maior = numeros[i];
        }
        else if (numeros[i] > segundo_maior)
        {
            segundo_maior = numeros[i];
        }
        else if (numeros[i] > terceiro_maior)
        {
            terceiro_maior = numeros[i];
        }
        else if (numeros[i] > quarto_maior)
        {
            quarto_maior = numeros[i];
        }
        else if (numeros[i] > quinto_maior)
        {
            quinto_maior = numeros[i];
        }
    }

    printf("\nMaior: %d", primeiro_maior);
    printf("\nsegundo maior: %d", segundo_maior);
    printf("\nterceiro maior: %d", terceiro_maior);
    printf("\nquarto maior: %d", quarto_maior);
    printf("\nquinto maior: %d", quinto_maior);
}