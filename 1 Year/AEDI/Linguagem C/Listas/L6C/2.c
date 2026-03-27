#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
 
 int case2();
 int matriz=0;
 int ordem=0;

 printf("\nOrdem matriz:\n");
  scanf("%d",&ordem);

switch(ordem){

 case 1:

 printf("\nnao existe sequencia\n");

 break;
 case 2:
 
 case2();
 
 break;
 case 3:

 break;

 default:
  printf("\nNao existe linhas\n");
 break;

}


	system("pause");
	return 0;
}

int case2(){

  int linha11=0,linha12=0,linha23=0,linha24=0;

  printf("\n[?][?]\n");
  scanf("%d",&linha11);
  scanf("%d",&linha12);

  printf("\n[%d][%d]\n",linha11,linha12);
  printf("\n[?][?]\n");
   scanf("%d",&linha23);
   scanf("%d",&linha24);

 int matriz[2][2] = {linha11,linha12,linha23,linha24};

 if(matriz[0][0] - matriz[0][1] == ){

 }

}