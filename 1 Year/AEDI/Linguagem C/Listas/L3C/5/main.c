#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int num=0,i;
 float media=0,nota[4];
 int cont_aprov=0,cont_reprov=0;
 
nota[0]=0;
nota[1]=0;
nota[2]=0;
nota[3]=0;

 for(i=1;i<=50;i++){

do{
   system("cls");
   printf("\nALUNO:%i\n",i);
   printf("\nNOTA 1:\n");
    scanf("%f",&nota[0]);
}while(nota[0]<0 || nota[0]>10); 
   system("cls");
do{
   system("cls");
   printf("\nALUNO:%i\n",i);
   printf("\nNOTA 2:\n");
    scanf("%f",&nota[1]);
}while(nota[1]<0 || nota[1]>10);
   system("cls");
do{
   system("cls");
   printf("\nALUNO:%i\n",i);
   printf("\nNOTA 3:\n");
    scanf("%f",&nota[2]);
}while(nota[2]<0 || nota[2]>10);
   system("cls");
do{
   system("cls");
   printf("\nALUNO:%i\n",i);
   printf("\nNOTA 4:\n");
    scanf("%f",&nota[3]);
}while(nota[3]<0 || nota[3]>10);

media = (nota[0]+nota[1]+nota[2]+nota[3])/4;

if(media>=7){
   printf("\nAPROVADO\n");
   cont_aprov++;
} 
if(media<7){
   printf("\nREPROVADO\n");
   cont_reprov++;
}



}
printf("\nPercentual Aprovados: %d\n",(cont_aprov*100)/50);
printf("\nPercentual Reprovados: %d\n",(cont_reprov*100)/50);








	system("pause");
	return 0;
}