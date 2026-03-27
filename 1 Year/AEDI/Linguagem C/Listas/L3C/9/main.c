#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int opc=0,num=0,i=1;
 float soma=0,maior=0,soma_mulheres=0;
 float altura=0,menor=0,soma_homens=0;



printf("\nNumeros de Alunos:\n");
 scanf("%d",&num);

while(i<=num){

do{
 printf("\n\t1 - MASCULINO\n\t2 - FEMININO\n");
  scanf("%d",&opc);
}while(opc!=1 && opc!=2); 

do{
 printf("\nAltura:\n");
  scanf("%f",&altura);

if(altura>=maior){
 	maior = altura;
}
if(altura<=menor){
 	menor = altura;
}

}while(altura<=0);;

if(opc==1){
  soma_homens+=altura;
}
if(opc==2){
  soma_mulheres+=altura;
}

 soma+=altura;

 i++;
}
printf("\nMEDIA DAS ALTURAS: %f\n",soma/num);
printf("\nMEDIA DAS MULHERES: %f\n",soma_mulheres/num);
printf("\nMEDIA HOMENS: %f\n",soma_homens/num);
printf("\nMAIOR ALTURA: %f\n",maior);
printf("\nMENOR ALTURA: %f\n",menor);







	system("pause");
	return 0;
}