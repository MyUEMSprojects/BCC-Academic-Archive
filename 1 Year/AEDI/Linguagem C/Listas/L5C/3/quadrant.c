#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
 
 char quadrante_primeiro[] = "primeiro";
 char quadrante_segundo[] = "segundo";
 char quadrante_terceiro[] = "terceiro";
 char quadrante_quarto[] = "quarto";

 int x=0,y=0;
 
 scanf("%d",&x);

 scanf("%d",&y);

 if(x>0 && y>0){
 	printf("\n%s\n",quadrante_primeiro);
 }
 if(x>0 && y<0){
 	printf("\n%s\n",quadrante_quarto);
 }
 if(x<0 && y<0){
    printf("\n%s\n",quadrante_terceiro);
 }
 if(x<0 && y>0){
 	printf("\n%s\n",quadrante_segundo);
 }
 if(x==0 && y>0){
 	printf("\n\n");
 }





	system("pause");
	return 0;
}