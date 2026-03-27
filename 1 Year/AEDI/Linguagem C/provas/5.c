#include <stdio.h>
#include <stdlib.h>

/* typedef struct
{
    char estado[50];
    int n_veic;
    int n_acid;
    int ano;
} ACID_TRANSITO; */
typedef struct
{
    char estado[50];
    int n_veic[11];
    int n_acid[11];
} ACID_TRANSITO;

int main(void)
{
    ACID_TRANSITO at[15];

    for (int i = 0; i < 15; i++)
    {
        printf("Estado:\n");
        scanf("%s", at[i].estado);
        printf("Numero Veiculo:\n");
        scanf("%d", at[i].n_veic);
        printf("Numero acid:\n");
        scanf("%d", at[i].n_acid);
        printf("Ano:\n");
        scanf("%d", at[i].ano);
    }

    system("pause");
    return 0;
}