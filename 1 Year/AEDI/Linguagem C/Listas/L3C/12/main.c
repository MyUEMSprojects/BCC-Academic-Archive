#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int controle=0,ingressos=120,num=5,i;
 float din=0,lucro=0,valor=5;

for(i=1;i<=8;i++){
 
 ingressos = ingressos + 26;
 valor = valor - 0.5;
 din = valor*ingressos;
 lucro = din - 200;
 
 printf("\nPara preco igual: %f\n",valor);
 printf("\nIngresso: %d\n",ingressos);
 printf("\nLucro Maximo: %f\n",lucro);
 printf("\n|-------Analise:%i-----------|\n",i);

}










	system("pause");
	return 0;
}