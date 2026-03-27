#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
 
 int soma=0,n=0;
 int num=0,resultado=0,i=1;

do{
system("cls");
printf("\nDigite a quantidade de Numeros:\n");
 scanf("%d",&n);
}while(n<0);

while(i<=n){

do{
system("cls");
printf("\nNUMERO: %i\n",i);
printf("\nNumero:\n");
 scanf("%d",&num);
}while(num<0);

 for (i = 1; i <= num / 2; i++) {
    if (num % i == 0) {
       resultado++;
    }
 }

if(resultado==0){
	soma+=num;
}

i++;
}
printf("\nSOMA DOS PRIMOS: %d\n",soma);





	system("pause");
	return 0;
}