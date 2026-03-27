#include <stdio.h>
#include <stdlib.h>

//Lista 4 exer5
//Felipe Echeverria Vilhalva
// Dados três conjuntos de números, de tamanho N, calcular a média entre suas médias.

int main(int argc, char const *argv[])
{
  int n = 1;
  int conjunto1[n];
  int conjunto2[n];
  int conjunto3[n];
  int soma1 = 0;
  int soma2 = 0;
  int soma3 = 0;
  float media = 0;
  float media1 = 0;
  float media2 = 0;
  float media3 = 0;

  do
  {
    printf("TAMANHO:\n");
    scanf("%d", &n);
  } while (n < 0);

  system("cls");

  for (int i = 1; i <= n; i++)
  {
    system("cls");
    printf("1-Valor[%i]:\n", i);
    scanf("%d", &conjunto1[i]);
    soma1 += conjunto1[i];
  }
  system("cls");

  for (int i = 1; i <= n; i++)
  {
    system("cls");
    printf("2-Valor[%i]:\n", i);
    scanf("%d", &conjunto2[i]);
    soma2 += conjunto2[i];
  }
  system("cls");

  for (int i = 1; i <= n; i++)
  {
    system("cls");
    printf("3-Valor[%i]:\n", i);
    scanf("%d", &conjunto3[i]);
    soma3 += conjunto3[i];
  }

  media1 = soma1 / n;
  media2 = soma2 / n;
  media3 = soma3 / n;

  media = (media1 + media2 + media3) / 3;

  printf("Media:%2.f\n", media);

  system("pause");
  return 0;
}