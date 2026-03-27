#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    int binario = 0;
    float binario2 = 0;
    int binario_certo[5];

    printf("NUMERO BINARIO:\n");
    scanf("%d", &binario);

    for (int i = 0; i < 10; ++i)
    {
        binario2 = binario / 10;
    }

    printf("%d\n", binario2);

    system("pause");
    return 0;
}
