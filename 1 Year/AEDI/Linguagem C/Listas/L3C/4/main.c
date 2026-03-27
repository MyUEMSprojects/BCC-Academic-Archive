\#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int nota=0,num=0,i;
 float conta=0,contb=0,contc=0,contd=0,conte=0;

 for(i=1;i<=30;i++){
  
do{
  printf("\nALUNO: %i\n",i);
  printf("\nNOTA:\n");
   scanf("%d",&nota);
}while(nota<0 || nota>100);

if(nota>=0 && nota<=39){
  printf("\nCONCEITO 'E'\n");
   conte++;
}
if(nota>=40 && nota<=49){
  printf("\nCONCEITO 'D'\n");
  contd++;
}
if(nota>=50 && nota<=74){
  printf("\nCONCEITO 'C'\n");
  contc++;
}
if(nota>=75 && nota<=89){
  printf("\nCONCEITO 'B'\n");
  contb++;
}
if(nota>=90 && nota<=100){
  printf("\nCONCEITO 'A'\n");
  conta++;
}


}
printf("\nPercentagem CONCEITO 'A': %2.f\n",(conta*100)/30);
printf("\nPercentagem CONCEITO 'B': %2.f\n",(contb*100)/30);
printf("\nPercentagem CONCEITO 'C': %2.f\n",(contc*100)/30);
printf("\nPercentagem CONCEITO 'D': %2.f\n",(contd*100)/30);
printf("\nPercentagem CONCEITO 'E': %2.f\n",(conte*100)/30);


	system("pause");
	return 0;
}