#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int cont = 1;
    int n = 1;
    int questoes[10];
    int codigos_alunos[n];
    int i = 0;

    codigos_alunos[0] = 1;

    while (i < cont)
    {
        printf("Codigo do aluno:\n");
        scanf("%d", &codigos_alunos[i]);

        if (codigos_alunos[i] != 9999)
        {
            ++cont;
        }
        ++i;
    }

    system("pause");
    return 0;
}
