#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{

 int palpite=0,num=0,cont=0,chances=3,i;


do{

 printf("\nPALPITE:\n");
  scanf("%d",&palpite);
  
 srand(time(NULL));

for(i=1;i<=1;i++){

	num = (rand()%99)+1;
}

if(palpite==num){
 printf("\nVOCE ACERTOU:\n");
   cont = 3;
}else{
    system("cls");
    cont++;
    chances--;
    printf("\nChances restantes: %d\n",chances);
	printf("\nVOCE ERROU tente novamente\n");
}


}while(cont<3);

system("cls");
printf("\nChanches esgotadas!\n");
printf("\nNumero: %d\n",num);







	system("pause");
	return 0;
}