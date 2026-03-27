#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// tipos de dados binarios
void tipos_dados()
{
    int8_t a = 127; // Valor maximo para int8_t
    int8_t b = - 128; // valor minimo para int8_t
    __int8_t c = 42; // Também funciona como int8_t
}

void bitwise_dados()
{
    __int8_t a = 0x55;
    __int8_t b = 0x0F;
}

int main()
{
    tipos_dados();
    return 0;
}
