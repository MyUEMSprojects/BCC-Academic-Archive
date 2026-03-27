#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> // Essa lib é necessaria para usar inet_addr
                       // permite converter um endereço IP em um formato limpo
#include <unistd.h>    // função para write

int main(int argc, char *argv[])
{
  int meu_socket, novo_socket, c;
  struct sockaddr_in servidor, cliente;
  char *mensagem;

  // criando socket
  meu_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(meu_socket == -1)
  {
    printf("Erro ao criar o socket!");
  }
  
  // preparando a estrutura sockaddr_in
  servidor.sin_family = AF_INET;
  servidor.sin_addr.s_addr = INADDR_ANY;
  servidor.sin_port = htons( 6666 );
  
  // Conectar a um servidor remoto
  if(bind(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
  {
    puts("Erro no bind");
    return 1;
  }

  puts("Bind executado corretamente");

  // Listen ouvindo conexões
  listen(meu_socket, 3);
  
  // teste com: telnet localhost 8888 em outro terminal
  // Aceitando conexões
  puts("Aguardando chegada de conexões...");
  c = sizeof(struct sockaddr_in);
  novo_socket = accept(meu_socket, (struct sockaddr *)&cliente, (socklen_t*)&c);
  if(novo_socket < 0)
  {
    perror("Erro ao aceita conexão");
    return 1;
  }

  puts("\nConexão aceita!");

  // Respondendo cliente
  mensagem = "Eae otario!\n";
  write(novo_socket, mensagem, strlen(mensagem));

  return 0;
}
