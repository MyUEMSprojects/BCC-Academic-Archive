#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTA 8080

int main(int argc, int *argv[])
{
    int cliente_fd, valor_lido, status;
    struct sockaddr_in serv_add;
    char *oi = "Bom dia do Cliente!";
    char buffer[1024] = {0};

    if ((cliente_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("erro no socket");
        exit(EXIT_FAILURE);
    }

    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(PORTA);

    // converte os endereços ipv4 e ipv6 de texto para a forma binária
    if (inet_pton(AF_INET, "127.0.0.1", &serv_add.sin_addr) < 0) // pton de protocolo to network
    {
        perror("Erro na conversão de endereços");
        exit(EXIT_FAILURE);
    }

    if ((status = connect(cliente_fd, (struct sockaddr *)&serv_add, sizeof(serv_add))) < 0)
    {
        printf("Erro no connect!\n");
        return -1;
    }

    send(cliente_fd, oi, strlen(oi), 0);
    printf("Mensagem de Bom Dia enviada!\n");
    valor_lido = read(cliente_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);

    // fechando conexao socket
    close(cliente_fd);

    return 0;
}