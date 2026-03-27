#include <stdio.h>
#include <stdlib.h>

/*
Alocação Dinâmica

1 - malloc
2 - calloc
3 - realloc
4 - free

*/

// Operador sizeof()

struct x
{
    int a;
    int b;
    int c;
};

int main(void)
{

    /* int x;
    double y; */
    int vetor[10];

    /* sizeof() tamanho em bytes daquele tipo de dado*/

    struct x estrutura;

    printf("%li", sizeof(estrutura)); /* pode colocar uma variavel ou o tipo de dado*/

    return 0;
}
