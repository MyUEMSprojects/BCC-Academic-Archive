#include <stdio.h>
#include <stdlib.h>

/* Dados dois conjuntos A e B de M e N posições, respectivamente, sendo N<=M, verificar se B está contido 
em A.  */

int main(void)
{
    int cont = 0;
    int m = 1, n = 1;
    int matrizA[m][n];
    int matrizB[m][n];

    do
    {
        system("cls");
        printf("Numero de linhas e colunas(MxN):\n");
        scanf("%d%d", &m, &n);
    } while (m > 15 || n > 15);

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            do
            {
                printf("MatrizA[%i][%i]:", i + 1, j + 1);
                scanf("%d", &matrizA[i][j]);
            } while (matrizA[i][j] < 0);
        }
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            do
            {
                printf("MatrizA[%i][%i]:", i + 1, j + 1);
                scanf("%d", &matrizB[i][j]);
            } while (matrizB[i][j] < 0);
        }
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            /* printf("MatrizA[%i][%i]: %d\n", i + 1, j + 1, matrizA[i][j]);
            printf("MatrizB[%i][%i]: %d\n", i + 1, j + 1, matrizB[i][j]); */

            if (matrizA[i][j] == matrizB[i][j])
            {
                cont++;
            }
        }
    }

    if (cont == m * n)
    {
        printf("Esta contido!\n");
    }
    else
    {
        printf("nao esta contido!\n");
    }

    system("pause");
    return 0;
}