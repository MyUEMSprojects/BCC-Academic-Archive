#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int i;
 float num=1,deno=0,soma=0,div=0;

 
 for(i=1;i<=10;i++){
    num = i;
if(i%2==0){
   	num = i*(-1);
}
   deno = i*i;
   div = num/deno;
   soma+=div;

}
printf("\nSOMA: %f\n",soma);






	system("pause");
	return 0;
}