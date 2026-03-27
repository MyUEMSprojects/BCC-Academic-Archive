#include <stdio.h>
#include <stdlib.h>

/* Uma determinada Universidade deseja saber se existem alunos cursando, simultaneamente, as disciplinas 
Programação de Computadores e Cálculo Numérico. Deverão ser conhecidos os números de matrícula dos 
alunos de PC (máximo 150) e de CN (máximo 220). O algoritmo deverá informar o número de matrícula dos 
alunos que estão cursando estas disciplinas simultaneamente.  */

int main(void)
{
    int cont = 1;
    int controle = 0;
    int controle_repetidos = 0;
    int control_main = 0;
    int alunos[cont];
    int alunos_simu[controle];
    int quantidade = 0;
    int curso[cont];
    int i = 0;

    do
    {

        do
        {
            system("cls");

            do
            {
                system("cls");
                printf("Matricula aluno %i:\n", i + 1);
                scanf("%d", &alunos[i]);

                for (int t = 0; t < cont - 1; ++t)
                {
                    if (alunos[i] == alunos[t])
                    {
                        controle_repetidos = 1;
                    }
                    else
                    {
                        controle_repetidos = 0;
                    }
                }

                if (controle_repetidos == 1)
                {
                    printf("Matricula repetida!\n");
                    system("pause");
                }

            } while (controle_repetidos != 0);

            do
            {
                system("cls");
                printf("Curso(PC ou CN ou PC\CN):\n");
                printf("\t1 - PC\n\t 2 - CN\n\t3 - PC\CN\n"); // 3 opcões
                scanf("%d", &curso[i]);

                if (curso[i] != 1 && curso[i] != 2 && curso[i] != 3)
                {
                    printf("curso invalido!\n");
                    system("pause");
                }
            } while (curso[i] != 1 && curso[i] != 2 && curso[i] != 3);

            if ((curso[i] == 1 && alunos[i] > 150) || (curso[i] == 2 && alunos[i] > 220))
            {
                printf("Matricula para curso invalido!\n");
                system("pause");
            }

        } while ((curso[i] == 1 && alunos[i] > 150) || (curso[i] == 2 && alunos[i] > 220));

        if (curso[i] == 3)
        {

            for (int j = controle; j < controle + 1; ++j)
            {
                alunos_simu[j] = alunos[i];
            }
            ++controle;
            ++quantidade;
        }

        do
        {

            system("cls");
            printf("Deseja inserir mais um aluno?\n");
            printf("\n\t1 - SIM\n\t2 - NAO\n");
            scanf("%d", &control_main);

            if (control_main != 2)
            {
                ++cont;
                ++i;
            }
        } while (control_main != 1 && control_main != 2);

        system("cls");

    } while (control_main != 2);

    if (quantidade != 0)
    {
        printf("Existem %d alunos cursando PC\CN simultaneamente\n", quantidade);

        for (int w = 0; w < controle; ++w)
        {
            printf("Aluno %i, Matricula: %d\n", w + 1, alunos_simu[w]);
        }
    }
    else
    {
        printf("Nao existem alunos cursando PC\CN simultaneamente!\n");
    }

    system("pause");
    return 0;
}