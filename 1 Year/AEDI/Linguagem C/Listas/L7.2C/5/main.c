/*
Faça um procedimento que receba horas, minutos e segundos por parâmetro e mostre esse tempo 
em segundos. 
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int horas = 0;
    int minutos = 0;
    int segundos = 0;
    int funcao();

    printf("\nHoras:\n");
    scanf("%d", &horas);
    printf("\nMinutos:\n");
    scanf("%d", &minutos);
    printf("\nSegundos:\n");
    scanf("%d", &segundos);

    funcao(horas, minutos, segundos);

    system("pause");
    return 0;
}
int funcao(int horas, int minutos, int segundos)
{
    int horas_segundos = 0;
    int minutos_segundos = 0;

    horas_segundos = (horas * 60) * 60;
    minutos_segundos = minutos * 60;

    printf("\nHoras pra segundos: %d\n", horas_segundos);
    printf("\nMinutos pra segundos: %d\n", minutos_segundos);
    printf("\nsegundos: %d\n", segundos);
}