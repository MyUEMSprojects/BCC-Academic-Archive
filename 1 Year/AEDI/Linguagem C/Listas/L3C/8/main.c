#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int contnum=0,cont=0,soma=0,i;


for(i=100;i<=500;i++){

if(i%2!=0 && i%3==0){
 	soma+=i;
 	cont++;
}

}
 printf("\nSOMA: %d\n",soma);
 printf("\nQuantidade: %d\n",cont);








	system("pause");
	return 0;
}