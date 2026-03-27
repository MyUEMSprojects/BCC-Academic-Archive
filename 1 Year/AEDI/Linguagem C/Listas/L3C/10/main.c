#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  
 int controle=0,i;
 float conversao=0;

 for(i=50;i<=150;i++){
do{
  system("cls");
  conversao = ((9*i)+32)/5;
  printf("\n%i graus em fahrenheit\n",i);
  printf("\nConversao: %f\n",conversao);

  printf("\nDigite 1 para ver novamente:\n");
  printf("\nDigite qualquer numero para continuar:\n");
   scanf("%d",&controle);
 }while(controle==1);
 }








	system("pause");
	return 0;
}