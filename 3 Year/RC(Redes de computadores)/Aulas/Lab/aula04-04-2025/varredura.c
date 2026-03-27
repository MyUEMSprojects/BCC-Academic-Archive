#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> // Essa lib é necessaria para usar inet_addr
                       // permite converter um endereço IP em um formato limpo
#include <unistd.h> // para poder usar a função close
/*
 * Criar um programa cliente que faça uma varredura em um servidor da web e mostre
 * as portas abertas e fechadas
 * [1...130] -> intervalo de portas
 *
 *
 *
 * */

int main(int argc, char *argv[])
{
  int porta_inicial = 1, porta_final = 130;
  
  return 0;
}
