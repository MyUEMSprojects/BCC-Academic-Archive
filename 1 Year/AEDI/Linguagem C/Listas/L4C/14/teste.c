#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int cont = 0;
    int cont_aprovados = 0;
    int acertos[cont];
    char respostas[10];
    char questoes[10];
    int codigos_alunos[cont];
    int i = 0;

    int porcentagem = 0;

    for (int w = 0; w < 10; ++w)
    {
        printf("Resposta da questao %i:\n", w + 1);
        scanf("%s", &respostas[w]);
    }

    do
    {
        acertos[i] = 0;

        printf("Codigo aluno %i\n", i);
        scanf("%d", &codigos_alunos[i]);

        for (int j = 0; j < 10; ++j)
        {
            printf("Questao %i:\n", j + 1);
            scanf("%s", &questoes[j]);
            s if (questoes[j] == respostas[j])
            {
                ++acertos[i];
            }
        }

        if (acertos[i] >= 6)
        {
            ++cont_aprovados;
        }

        printf("Aluno %i, acertos: %d\n", i, acertos[i]);

        if (codigos_alunos[i] != 9999)
        {
            ++cont;
            ++i;
        }

    } while (codigos_alunos[i] != 9999);

    porcentagem = ((cont_aprovados + 1) * 100) / cont;

    printf("Foram aprovados %d da turma\n", porcentagem);

    system("pause");
    return 0;
}
