#include <stdio.h>
#include <stdlib.h>

//Lista 4 exer1
//Felipe Echeverria Vilhalva

int main(int argc, char const *argv[])
{
 
 int numero[30]={0};
 int menor,maior=0;

 for(int i=1;i<=6;i++){
  printf("NUMERO[%i]\n",i);
  scanf("%d",&numero[i]);
  
  if(numero[i] >= maior){
     maior = numero[i];
  }
  if(numero[i] <= menor){
    menor = numero[i];
  }

 }

 printf("MAIOR: %d\n",maior);
 printf("MENOR: %d\n",menor);






  system("pause");
  return 0;
}