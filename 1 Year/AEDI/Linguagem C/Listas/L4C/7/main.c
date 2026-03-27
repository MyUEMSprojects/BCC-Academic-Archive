#include <stdio.h>
#include <stdlib.h>

//Lista 4 exer7
//Felipe Echeverria Vilhalva
/*Encontrar o maior elemento e suas respectivas posições, de um vetor A com 1000 elementos. As posições 
do maior elemento devem ser armazenadas no vetor POSICAO e este deverá ser informado no final do 
algoritmo.*/

int main(int argc, char const *argv[])
{

  int funcao();

  funcao();

  system("pause");
  return 0;
}

int funcao()
{

  int VetorA[10];
  int Vetor_posicao[10];
  int maior = 0;

  for (int i = 0; i < 10; i++)
  {
    printf("Vetor[%i]\n", i);
    scanf("%d", &VetorA[i]);

    if (VetorA[i] >= maior)
    {
      Vetor_posicao[i] = VetorA[i];
      maior = Vetor_posicao[i];
    }
  }

  printf("Maior: %d\n", maior);
}