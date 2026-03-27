#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int num1 = 0;
    int num2 = 0;
    int opc = 0;
    int resposta = 0;
    int funcao();

    do
    {
        system("cls");
        printf("\n\t1 - Adicao\n\t2 - Subtracao\n\t3 - Multiplicacao\n\t4 - Divisao");
        scanf("%d", &opc);
    } while (opc != 1 && opc != 2 && opc != 3 && opc != 4);

    system("cls");

    printf("\nVALOR 1:");
    scanf("%f", &num1);
    printf("\nVALOR 2:");
    scanf("%f", &num2);

    resposta = funcao(num1, num2, opc);

    printf("\nresposta = %f\n", resposta);

    system("pause");
    return 0;
}

int funcao(int num1, int num2, int opc)
{
    switch (opc)
    {
    case 1:

        return num1 + num2;

        break;
    case 2:

        return num1 - num2;

        break;
    case 3:

        return num1 * num2;

        break;

    case 4:

        return num1 / num2;

        break;
    default:

        return NULL;

        break;
    }
}