#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
  int meu_socket, b;
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // preenche automaticamente o IP
   
  getaddrinfo(NULL, "3490", &hints, &res);
 
  // criar o socket
  meu_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if(meu_socket == -1)
  {
    printf("Erro ao criar o socket!");
  }
  else
  {
    printf("\nSocket criado com sucesso!");
  }

  b = bind(meu_socket, res->ai_addr, res->ai_addrlen);

  if(b == -1)
  {
    printf("Erro ao criar o bind\n");
  }
  else
  {
    printf("Bind associado com sucesso");
  }

  return 0;
}
