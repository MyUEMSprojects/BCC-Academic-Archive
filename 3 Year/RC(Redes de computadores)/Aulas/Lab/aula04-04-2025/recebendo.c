#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> // Essa lib é necessaria para usar inet_addr
                       // permite converter um endereço IP em um formato limpo
#include <unistd.h> // para poder usar a função close

int main(int argc, char *argv[])
{
  int meu_socket;
  struct sockaddr_in servidor;
  char *mensagem, resposta_servidor[2000];

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
  puts("conectado com sucesso");

  // enviando alguns dados
  mensagem = "GET / HTTP/1.1\r\n\r\n"; // comando para buscar a página principal do site
  if( send(meu_socket, mensagem, strlen(mensagem), 0) < 0)
  {
    puts("Falha ao enviar!");
    return 1;
  }

  puts("Dados enviados com sucesso\n");

  // Recebendo resposta do servidor
  if(recv(meu_socket, resposta_servidor, 2000, 0) < 0)
  {
    puts("Falha ao receber a resposta!");
    return 1;
  }

  puts("Reposta recebida!\n");
  puts(resposta_servidor);

  close(meu_socket);
  return 0;
}
