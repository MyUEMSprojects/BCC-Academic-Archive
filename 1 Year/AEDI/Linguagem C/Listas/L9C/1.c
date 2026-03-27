#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char contarVogais(char *frase);
    int resposta = 0;
    char frase[] = " ";

    scanf("%s", frase);

    resposta = contarVogais(frase);

    printf("%d\n", resposta);

    system("pause");
    return 0;
}

char contarVogais(char *frase)
{
    int cont = 0;
    for (int i = 0; i < sizeof(frase) / sizeof(int); i++)
    {
        if ((frase[i] == 'a') || (frase[i] = 'A'))
        {
            cont++;
        }
        else if ((frase[i] == 'e') || (frase[i] = 'E'))
        {
            cont++;
        }
        else if ((frase[i] == 'i') || (frase[i] = 'I'))
        {
            cont++;
        }
        else if ((frase[i] == 'o') || (frase[i] = 'O'))
        {
            cont++;
        }
        else if ((frase[i] == 'u') || (frase[i] = 'U'))
        {
            cont++;
        }
        else
        {
            printf("\n");
        }
    }
    return cont;
}