#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define tam 50

//Lista 4 exer4 
//Felipe Echeverria Vilhalva

int main(int argc, char const *argv[])
{
 
 int calculo;
 int numero[tam];

    for(int i=1;i<=tam;i++){
    system("cls");

      printf("NUMERO[%i]\n",i);
      scanf("%d",&numero[i]);

        calculo = pow(numero[i],i);

      printf("%i elevado a %i eh: %d\n",numero[i],i,calculo);

      system("pause");
    }





	system("pause");
	return 0;
}