#include <stdio.h>

// somar elementos de forma sequecial
int soma(int *s, int inicio, int fim)
{
    if(inicio > fim)
    {
        return 0;
    }

    return s[inicio] + soma(s, inicio + 1, fim);
}

// somar elementos do meio pra tras + do meio pra frente
int soma_(int *s, int inicio, int fim){
    
    
    if(inicio == fim) {
        return s[inicio];
    }

    int meio = (inicio + fim) / 2;
    return soma(s, inicio, meio) + soma(s, meio + 1, fim);
}

int main() {
    int array[] = {1, 2, 3, 4, 5};
    int tamanho = sizeof(array) / sizeof(array[0]);
    
    int resultado = soma(array, 0, tamanho - 1);
    int resultado_ = soma_(array, 0, tamanho - 1);
    printf("A soma dos elementos(sequencial) do array é: %d\n", resultado);
    printf("A soma dos elementos(meio_divisor) do array é: %d\n", resultado_);
    
    return 0;
}