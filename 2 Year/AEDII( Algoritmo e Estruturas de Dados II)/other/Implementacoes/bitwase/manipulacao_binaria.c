#include <stdio.h>
#include <stdlib.h>

// Acessando bytes individuais de uma variável:

void acessando_bytes_individuais()
{
    int num = 0x12345678; // Exemplo de número hexadecimal (4 bytes)
    unsigned char *byte_ptr = (unsigned char *)&num;

    printf("Valor de num: 0x%x\n", num);

    // Acessando os 4 bytes de um inteiro (assumindo little-endian)
    for (int i = 0; i < sizeof(int); i++)
    {
        printf("Byte %d: 0x%x\n", i, byte_ptr[i]);
    }
}

// Operações bitwise para manipular bytes:

void bitwise_manipular_bytes()
{
    int num = 0x12345678;
    unsigned char second_byte = (num >> 8) & 0xFF; // Desloca 8 bits e aplica máscara de 1 byte (0xFF)
    // 0xFF = 255(dec) = 1111 1111
    
    printf("Segundo byte: 0x%x\n", second_byte); // Esperado: 0x56
}

// Manipulando bytes em buffers:
void buffer_bytes()
{
    unsigned char buffer[4] = {0x12, 0x34, 0x56, 0x78}; // Buffer de 4 bytes

    printf("Buffer original:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Byte %d: 0x%x\n", i, buffer[i]);
    }

    // Alterar o terceiro byte para 0xFF
    buffer[2] = 0xFF;

    printf("\nBuffer após alteração:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Byte %d: 0x%x\n", i, buffer[i]);
    }
}

/*
Ao manipular bytes diretamente, é importante estar ciente de como a máquina armazena os bytes na memória: little-endian ou big-endian. Isso afeta a ordem dos bytes ao acessar variáveis multi-byte.

    Little-endian: O byte menos significativo vem primeiro (ex.: 0x12345678 seria armazenado como 78 56 34 12).
    Big-endian: O byte mais significativo vem primeiro (ex.: 0x12345678 seria armazenado como 12 34 56 78).
*/

void bytes_armazena_endian()
{
    unsigned int x = 1;
    char *c = (char *)&x; // Little-endian 

    if (*c)
    {
        printf("Little-endian\n");
    }
    else
    {
        printf("Big-endian\n");
    }
}

union Data
{
    int num;
    unsigned char bytes[4];
};

/*
Em C, você também pode usar structs e unions para acessar bytes específicos de uma estrutura de dados maior. 
Usar uma union é útil porque permite que você visualize os mesmos dados de maneiras diferentes (como um inteiro ou como bytes).
*/
void byte_union_struct()
{
    union Data data;
    data.num = 0x12345678;

    printf("Acessando os bytes da union:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Byte %d: 0x%x\n", i, data.bytes[i]);
    }
}

/*
Manipular bytes também é muito comum ao trabalhar com arquivos binários em C. 
Por exemplo, ler e escrever bytes diretamente de arquivos usando fread e fwrite.
*/
void write_read_byte_file()
{
    unsigned char data[4] = {0x12, 0x34, 0x56, 0x78};

    // Escrevendo bytes em um arquivo binário
    FILE *file = fopen("data.bin", "wb");
    fwrite(data, sizeof(unsigned char), 4, file);
    fclose(file);

    // Lendo bytes do arquivo binário
    unsigned char read_data[4];
    file = fopen("data.bin", "rb");
    fread(read_data, sizeof(unsigned char), 4, file);
    fclose(file);

    printf("Bytes lidos:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Byte %d: 0x%x\n", i, read_data[i]);
    }
}

int main()
{
    acessando_bytes_individuais();
    printf("\n");
    bitwise_manipular_bytes();
    printf("\n");
    buffer_bytes();
    printf("\n");
    bytes_armazena_endian();
    printf("\n");
    byte_union_struct();
    printf("\n");
    write_read_byte_file();
    printf("\n");
    return 0;
}