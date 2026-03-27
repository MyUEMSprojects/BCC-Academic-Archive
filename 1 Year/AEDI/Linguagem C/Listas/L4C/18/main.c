#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char nomes[2][10];

    for (int i = 0; i < 2; ++i)
    {
        scanf("%s", &nomes[i]);
    }

    for (int j = 0; j < 2; ++j)
    {
        for (int w = 0; w < 2; ++w)
        {
            nomes[j] = nomes[w];
        }
    }

    system("pause");
    return 0;
}
