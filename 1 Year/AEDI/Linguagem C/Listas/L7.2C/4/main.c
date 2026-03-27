/*
Faça um procedimento que receba dois números positivos por parâmetro e mostre a soma dos N 
números inteiros existentes entre eles. 
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int num1;
    int num2;
    int resposta = 0;
    int funcao();
    do
    {
        system("cls");
        scanf("%d %d", &num1, &num2);
    } while (num1 < 0 || num2 < 0);
    resposta = funcao(num1, num2);

    printf("\nresposta = %d\n", resposta);

    system("pause");
    return 0;
}
int funcao(int num1, int num2)
{
    int soma = 0;

    if (num1 < num2)
    {
        for (int i = num1; i <= num2; i++)
        {
            soma += i;
        }
    }
    else
    {
        for (int i = num1; i >= num2; i--)
        {
            soma += i;
        }
    }
    return soma;
}