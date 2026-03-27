#include <stdio.h>
#include <math.h>

// Busca em uma lista ordenada
int busca(int *s, int inicio, int fim, int chave)
{
    if(inicio <= fim)
    {
        int meio = (inicio + fim)/2;

        if(s[meio] == chave)
        {
            //return meio;
            return s[meio];
        }
        else if(chave < s[meio])
        {
            return busca(s, inicio, meio -1, chave);
        }
        else
        {
            return busca(s, meio + 1, fim, chave);
        }
    }

    return -1;
}

int main()
{
    int vetor[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);
    int chave = 12; // encontrada(12)
    //chave = 21; // nao encontrada(-1)
    printf("%d", busca(vetor, 0, tamanho -1, chave));

    return 0;
}