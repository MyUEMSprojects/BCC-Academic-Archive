#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{

 int palpite=0,num=0,i;
  
  printf("\nPALPITE:\n");
 	scanf("%d",&palpite);

  srand(time(NULL));

for(i=1;i<=1;i++){

   num = (rand()%99)+1;
} 

if(palpite==num){
	printf("\nVoce acertou!\n");
}else{
	printf("\nVoce errou\n");
}




	system("pause");
	return 0;
}