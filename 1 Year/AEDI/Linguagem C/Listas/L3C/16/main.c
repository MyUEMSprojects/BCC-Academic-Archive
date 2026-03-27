#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
 
 int controle=0,i;
 float a=0,b=0,c=0;
 float soma=0,num=0;

do{

do{
system("cls");
printf("\nA:\n");
 scanf("%f",&a);
}while(a!=(int)a || a<1);

system("cls");

do{
system("cls");
printf("\nB:\n");
 scanf("%f",&b);
}while(b!=(int)b);

system("cls");

do{
system("cls");
printf("\nC:\n");
 scanf("%f",&c);
}while(c!=(int)c);

for(i=b;i<=c;i++){
  
 soma+=i;
if(soma<0){
  soma = soma * (-1);
}

}
printf("\nSOMA: %f\n",soma);

//__SE FODA__\\
\\CODIGO LIXO//




printf("\nDigite 1 para continuar:\n");
 scanf("%d",&controle);
}while(controle==1);


	system("pause");
	return 0;
}