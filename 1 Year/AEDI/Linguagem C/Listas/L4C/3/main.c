#include <stdio.h>
#include <stdlib.h>

//Lista 4 exer3
//Felipe Echeverria Vilhalva

int main(int argc, char const *argv[])
{

 int tam = 0;
 int n[tam];
 int i;
 int soma=0;
 
 printf("Tamanho:\n");
  scanf("%d",&tam);

 
 for(i=1;i<=tam;i++){
   printf("ELEMENTO[%i]:\n",i);
    scanf("%d",&n[i]);
    soma+=n[i];
 }

 printf("SOMA: %d\n",soma);





   system("pause");
   return 0;
}