#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int cont = 1;
    int acertos[cont];
    char respostas[10];
    char questoes[10];
    int codigos_alunos[cont];
    int i = 0;
    int k = 0;
    int u = 0;

    codigos_alunos[0] = 1;
    acertos[0] = 0;

    for (int j = 0; j < 10; ++j)
    {
        printf("Resposta da questao %i\n", j + 1);
        scanf("%s", &respostas[j]);
    }

    while (i < cont)
    {

        printf("Codigo do aluno:\n");
        scanf("%d", &codigos_alunos[i]);

        if (codigos_alunos[i] != 9999)
        {
            while (k < 10)
            {

                printf("Questao %i\n", k + 1);
                scanf("%s", &questoes[k]);

                if (questoes[k] == respostas[k])
                {
                    ++acertos[i];
                }
                ++k;
            }
            ++cont;
        }
        else
        {
            while (u < 10)
            {
                printf("Questao %i\n", u + 1);
                scanf("%s", &questoes[u]);
                ++u;
            }
        }

        /* printf("Aluno %i, acertos: %d/10 \n", i + 1, acertos[i]); */

        ++i;
    }

    system("pause");
    return 0;
}
