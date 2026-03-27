#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int controle=0,control=0,opc=0;
 float cont=3,cont_branco=1,cont_tinto=1,cont_rose=1;
 

 printf("\nAdicionar mais vinho?\n");
 printf("\n\t1 -  SIM\n\t2 - NAO\n");
  scanf("%d",&controle);

if(controle==1){

 do{
  system("cls");
do{
  system("cls");
 printf("\nAdicionar vinhos:\n");
 printf("\n\t1 - BRANCO\n\t2 - TINTO\n\t3 - ROSE\n");
   scanf("%d",&opc);
}while(opc!=1 && opc!=2 && opc!=3);

if(opc==1){
  cont_branco++;
}
if(opc==2){
  cont_tinto++;
}
if(opc==3){
  cont_rose++;
}
  cont++;

 printf("\nAdicionar mais vinho?\n");
 printf("\n\t1 -  SIM\n\t2 - NAO\n");
  scanf("%d",&control);
}while(control==1);
 printf("\nPercentual VINHO BRANCO: %f\n",(cont_branco*100)/cont);
 printf("\nPercentual VINHO TINTO: %f\n",(cont_tinto*100)/cont);
 printf("\nPercentual VINHO ROSE: %f\n",(cont_rose*100)/cont);

 //--------0__________0----------\\

}else{
 printf("\nPercentual VINHO BRANCO: %f\n",(cont_branco*100)/cont);
 printf("\nPercentual VINHO TINTO: %f\n",(cont_tinto*100)/cont);
 printf("\nPercentual VINHO ROSE: %f\n",(cont_rose*100)/cont);
}


  system("pause");
   return 0;
}
