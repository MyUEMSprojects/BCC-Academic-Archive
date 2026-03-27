#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int multi();
    int resultado;

    multi();

    system("pause");
    return 0;
}
int multi(int numero1, int numero2)
{
    scanf("%d %d", &numero1, &numero2);

    printf("%d", numero1 * numero2);
}