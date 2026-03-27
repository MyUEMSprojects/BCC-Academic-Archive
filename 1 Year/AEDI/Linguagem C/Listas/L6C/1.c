#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
 
 int primeiraordem();
 int segundaordem();
 int terceiraordem();
 int ordem=0;
 

 printf("\nDigite a ordem a matriz:\n");
  scanf("%d",&ordem);


  switch(ordem){
    
    case 1:
      
     primeiraordem();

    break;

  	case 2:
     
     segundaordem();
      
  	break;

  	case 3:
     
    terceiraordem();

  	break;


  	default: 
      printf("\nOrdem invalida\n");
  	break;
  }
   

	system("pause");
	return 0;
}

int primeiraordem(){
      
      int matriz[3][3] = {1,2,3,4,5,6,7,8,9};

      printf("\ndeterminante = %i\n",matriz[0][0]);
   }

int segundaordem(){
    
    int resultado = 0;
    int matriz[2][2] = {1,2,3,4};
    
    resultado = (matriz[0][0] * matriz[1][1]) - (matriz[1][0] * matriz[0][1]);


    printf("\ndeterminante = %i\n",resultado); 

}

int terceiraordem(){

 int resultado = 0;
 int resultado_diagonal = 0;
 int resultado_lado = 0;
 int matriz[3][3] = {1,2,3,4,5,6,7,8,9};
  
 resultado_diagonal = ((matriz[0][0] * matriz[0][4] * matriz[0][8]) + (matriz[0][1] * matriz[0][5] * matriz[0][6])+(matriz[0][2] * matriz[0][3] * matriz[0][7]));
 resultado_lado = ((matriz[0][1] * matriz[0][3] * matriz[0][8])+(matriz[0][0] * matriz[0][5] * matriz[0][7])+(matriz[0][2] * matriz[0][4] * matriz[0][6]));

 resultado = resultado_diagonal - resultado_lado;

  printf("\ndeterminante = %i\n", resultado_lado);

}