#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AAA(char *FRASE, char C)
{
    char s[strlen(FRASE) + 1];
    int i, j = 0;

    for (i = 0; i < strlen(FRASE); i++)
    {
        if (FRASE[i] != C)
        {
            s[j] = FRASE[i];
            j++;
        }
    }
    s[j] = '\0';
    strcpy(FRASE, s);

    printf("%s\n", s);
    printf("%d\n", j);
}

int main(void)
{
    char frase[] = "Felipe";
    char palavra = 'i';

    AAA(frase, palavra);

    system("pause");
    return 0;
}