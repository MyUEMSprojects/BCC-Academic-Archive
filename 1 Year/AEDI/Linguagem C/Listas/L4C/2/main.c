#include <stdio.h>
#include <stdlib.h>

//Lista 4 exer2
//Felipe Echeverria Vilhalva

int main(int argc, char const *argv[])
{
 
 int maior=0;
 int menor;
 int n=1;
 int numero[n];
 
  do{
    scanf("%d",&n);
  }while(n<0);

  for(int i=1;i<=n;i++){
  printf("Numero[%i]\n",i);
    scanf("%d",&numero[i]);
      numero[i+1] = 999;
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