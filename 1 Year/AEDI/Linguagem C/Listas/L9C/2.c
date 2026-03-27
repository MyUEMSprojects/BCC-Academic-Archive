#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int conta_vogais(const char *s);

int main()
{
    char frase[100];

    printf("Digite uma frase\n");
    fgets(frase, 100, stdin);

    conta_vogais(frase);
}

int conta_vogais(const char *s)
{
    int i, num = 0;
    char vogais[10] = "AaEeIiOoUu";

    while (*s)
    {
        for (i = 0; i < 10; i++)
            if (*s == vogais[i])
                num++;
        s++;
    }

    printf(" Quantidade de Vogais == %d\n", num);
}