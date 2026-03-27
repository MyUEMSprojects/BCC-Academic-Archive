#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int n=0,i,j,linha=0,cont=0;

 do{ 
 system("cls");
 scanf("%d",&n);
 }while(n<1 || n>100);
 
 cont = n*2;

 for(i=1;i<=cont;i++){
  
  for(j=1;j<=3;j++){
     linha = n*2;
    printf("\n%d\n",linha);
  }

 printf("\n%i\n",i);

 }







    system("pause");
    return 0;
}