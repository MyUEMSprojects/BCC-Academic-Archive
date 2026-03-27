#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTA 8080

int main(int argc, char *argv[])
{
    int servidor_fd, novo_socket; // servidor_fd = server file descriptor
    ssize_t valor_lido;
    struct sockaddr_in address;
    int opt = 1; // flag
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *oi = "Bom dia do Servidor";

    // Criando um socket descriptor. TUdo em Unix é feito usando files
    // File descriptor é um inteiro que se refere a esse file.
    if ((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Falha no socket");
        exit(EXIT_FAILURE);
    }

    // anexando o socket a porta 8080
    if (setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Problema no setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORTA);

    // Anexando o socket à porta 8080
    if (bind(servidor_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Falha no bind");
        exit(EXIT_FAILURE);
    }

    if (listen(servidor_fd, 3) < 0)
    {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    if ((novo_socket = accept(servidor_fd, (struct sockaddr *)&address, &addrlen)) < 0)
    {
        perror("Erro no accept");
        exit(EXIT_FAILURE);
    }

    valor_lido = read(novo_socket, buffer, 1024 - 1);
    // subtrai 1 por conta do terminador nulo do final;
    printf("%s\n", buffer);
    send(novo_socket, oi, strlen(oi), 0);
    printf("Mensagem de Bom Dia enviada!\n");

    // fechando conexão
    close(novo_socket);
    // fechando o listen socket
    close(servidor_fd);

    return 0;
}