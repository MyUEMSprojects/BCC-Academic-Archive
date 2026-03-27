#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> // Essa lib é necessaria para usar inet_addr
                       // permite converter um endereço IP em um formato limpo

int main(int argc, char *argv[])
{
  int meu_socket;
  struct sockaddr_in servidor;
  char *mensagem;

  // criando socket
  meu_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(meu_socket == -1)
  {
    printf("Erro ao criar o socket!");
  }

  servidor.sin_addr.s_addr = inet_addr("142.250.219.227"); // endereço do google brasil
  servidor.sin_family = AF_INET;
  servidor.sin_port = htons( 80 );

  // Conectar a um servidor remoto
  if(connect(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
  {
    puts("Erro ao conectar");
  }
  puts("conectado");

  // enviando alguns dados
  mensagem = "GET / HTTP/1.1\r\n\r\n"; // comando para buscar a página principal do site
  if( send(meu_socket, mensagem, strlen(mensagem), 0) < 0)
  {
    puts("Falha ao enviar!");
    return 1;
  }

  puts("Dados enviados\n");
  return 0;
}
