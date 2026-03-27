#include <stdio.h>
#include <stdlib.h>

// Dado um conjunto de N elementos, encontrar o menor elemento dos maiores que 10.

int main(void)
{
    int n = 1;
    int menor;
    int elementos[n];

    printf("numero de elementos:\n");
    scanf("%d", &n);

    for (int i = 0; i < n; ++i)
    {
        printf("elemento[%i]\n", i + 1);
        scanf("%d", &elementos[i]);
        if (elementos[i] > 10)
        {
            if (elementos[i] < menor)
            {
                menor = elementos[i];
            }
        }
    }

    printf("Menor elementos entre os maiores que 10: %d\n", menor);

    system("pause");
    return 0;
}
