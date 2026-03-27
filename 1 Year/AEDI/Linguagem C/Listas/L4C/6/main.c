#include <stdio.h>
#include <stdlib.h>

//Lista 4 exer6
//Felipe Echeverria Vilhalva
/*Encontrar o maior elemento e sua respectiva posição, de um vetor A com 1000 elementos. Não existem 
elementos repetidos*/

int main(int argc, char const *argv[])
{
 
 int posicao=0;
 int maior=0;
 int A[1000];
  
 for(int i=0;i<1000;i++){
  printf("A[%i]:\n",i+1);
 	scanf("%d",&A[i]);

    if(A[i] >= maior){
    	maior = A[i];
    	posicao = i+1;
    }
 }
 
 printf("MAIOR: %d\n",maior);
 printf("Posicao[%d]\n",posicao);







	system("pause");
	return 0;
}