#include <stdio.h>
#include <sys/socket.h>

// esse é um socket criado para TCP
int main(int argc, char **argv)
{
    int meu_socket;
    meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (meu_socket == -1)
    {
        printf("\nErro ao criar socket!\n");
    }
    else
    {
        printf("\nSocket criado com sucesso!\n");
    }
    return 0;
}
