#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int funcao();
    int num1;
    int num2;

    scanf("%d %d", &num1, &num2);

    funcao(num1, num2);

    system("pause");
    return 0;
}

int funcao(int numero1, int numero2)
{
    float calculo = 0;
    calculo = numero1 / numero2;

    printf("%f", calculo);
}