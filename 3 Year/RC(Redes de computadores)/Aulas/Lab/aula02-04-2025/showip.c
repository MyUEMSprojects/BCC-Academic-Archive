#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];
  
  if(argc != 2) {
    fprintf(stderr, "Usar: showip hostname\n"); // fprintf é usado para streaming de dados 
    return -1;
  }
  
  memset(&hints, 0, sizeof hints); // limpar struct
  hints.ai_family = AF_UNSPEC; // server tanto para AF_INET ou AF_INET6
  hints.ai_socktype = SOCK_STREAM; // TCP
  
  if((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }
  
  printf("Endereço IP para %s:\n\n", argv[1]);
  
  for(p = res; p != NULL; p = p->ai_next)
  {
    void *addr; // quando nao sabe o tipo que esta retornando voce usa void
    char *ipver;
    // pega o ponteiro do proprio endereço
    // diferentees campos ipv4 e ipv6

    // precisa saber se é ipv4 ou ipv6
    if(p->ai_family == AF_INET)
    {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr; // endereço do socket de internet
      addr = &(ipv4->sin_addr); // sin
      ipver = "IPv4";
    }
    else // ipv6
    {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;  // Fixed this line
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    
    // Big Endian and Little Endian
    // converte ip para uma string para imprimir
    // ntop -> network to protocol
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf(" %s: %s\n", ipver, ipstr);
  }
  
  freeaddrinfo(res); // Libera a lista
  return 0;
}
