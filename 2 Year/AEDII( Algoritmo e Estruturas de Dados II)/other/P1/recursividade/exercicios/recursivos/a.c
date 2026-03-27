#include <stdio.h>

void inverter(int *s, int incio, int fim){
     
    if(incio >= fim) {
        return;
    }

    int temp = s[incio];
    s[incio] = s[fim];
    s[fim] = temp;

    inverter(s, incio + 1, fim - 1);
}

int main(){
    
    int vetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);
    inverter(vetor, 0, tamanho - 1);

    for (int i = 0; i < tamanho; i++)
    {
        printf("%d\n", vetor[i]);
    }
    

    return 0;
}