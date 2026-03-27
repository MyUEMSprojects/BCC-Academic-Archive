#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Deslocamento á esquerda
    int a, b;
    //a = 1; // 0001
    //b = a << 1; // 0010 = 2
    //printf("a: %d\tb: %d\n", a, b);

    a = 1;
    b = ~a;

    printf("\ta: %d\t!a: %d\n", a, b);


    
    return 0;
}