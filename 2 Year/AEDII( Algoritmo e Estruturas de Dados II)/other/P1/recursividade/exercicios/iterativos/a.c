#include <stdio.h>

void inverter(int *s, int tamanho) {
    int i = 0;
    int j = tamanho - 1;

    while (i < j) {
        int aux = s[i];
        s[i] = s[j];
        s[j] = aux;
        i++;
        j--;
    }
}

int main(){
    int vetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);
    inverter(vetor, tamanho);
    
    for (int i = 0; i < tamanho; i++) {
        printf("%d\n", vetor[i]);
    }

    return 0;
}