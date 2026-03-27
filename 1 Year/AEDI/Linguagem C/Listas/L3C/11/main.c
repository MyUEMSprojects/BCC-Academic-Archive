#include <stdio.h>
#include <stdlib.h>
//copyright:: FELIPE ECHEVERRIA VILHALVA//
//Github: https://github.com/rrozdoce//
int main(int argc, char const *argv[])
{
 
 char sexo;
 int controle=0,meses=0,opc=0,num=0,i=1;
 float mortas=0,cont_morta=0,cont_viva=0,cont=0;
 float cont_meses=0,cont_masc=0,cont_fem=0;

do{
do{
 system("cls");
 printf("\nNumero de Criancas:\n");
  scanf("%d",&num);
}while(num<=0);

while(i<=num){

do{
 system("cls");
 printf("\nCrianca: %i\n",i);
 printf("\n\t1 -  Morta\n\t2 - VIVA\n");
  scanf("%d",&opc);
}while(opc!=1 && opc!=2);
  system("cls");
if(opc==1){

do{ 
 system("cls");
 printf("\nCrianca: %i\n",i);
 printf("\nSEXO:\n");
  scanf("%c",&sexo);

if(sexo=='M'){
  cont_masc++;
}
if(sexo=='F'){
  cont_fem++;
}

}while(sexo!='M' && sexo!='F');
system("cls");
do{
 system("cls");
 printf("\nCrianca: %i\n",i);
 printf("\nMeses de vida:\n");
  scanf("%d",&meses);

if(meses<=24){
 cont_meses++;
}
}while(meses<0);

 cont_morta++;
}else{
  cont_viva++;
}

  cont++;
i++;
}
 mortas = (cont_morta*100)/num;
printf("\nPercentagem criancas masculinas mortas: %f\n",(cont_masc*100)/num);
printf("\nPercentagem Criancas Mortas: %f\n",mortas);
printf("\nPercentagem Criancas viveram menos de 24 meses: %f\n",(cont_meses*mortas)/num);

//--------0________0---------\\
\\___TALVEZ CONTENHA ERROS___//
do{
 printf("\nDigite:\n");
 printf("\n\t1 - Continuar programa\n\t2 - Encerrar programa\n");
  scanf("%d",&controle);
}while(controle!=1 && controle!=2);

}while(controle==1);




	system("pause");
	return 0;
}