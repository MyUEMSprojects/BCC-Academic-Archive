#include <stdio.h>
#include <math.h>
#include <time.h>

/*
Implementar um algoritmo que calcule a raiz para funções polinomiais pelo método da Bissecção.

Entrada: Grau do polinômio, coeficientes, intervalo que contém a raiz, erro.

Saída: Print das respostas de cada iteração, solução, erro final e tempo de execução.

*/

double polinomio(double coeficientes[], int grau, int x) {

   double operacao = 0.0;  

   for(int i = 0; i <= grau; i++) {

     operacao += coeficientes[i] * pow(x, i);
       
   }

   return operacao;
}

double bisseccao(int grau, double coeficientes[grau], double intervalo[2], double erro) {
  
   double xn, aux_xn, aux_fn, delta = 1, fxn;
   int i = 0;
   
   while(delta > erro) {
    
    if(i == 0) {
     xn = (coeficientes[0] + coeficientes[1]) / 2;
     fxn = polinomio(coeficientes, grau, xn);
      
     coeficientes[1] = xn;

     aux_xn = xn;
     aux_fn = fxn;

     printf("Interacao %d\n", i);
     printf("[%lf, %lf]\n", intervalo[0], intervalo[1]);
     printf("xn: %.7lf\n", xn);
     printf("fxn: %.7lf\n", fxn);
     printf("delta: ----\n");

   
    }else {

     xn = (coeficientes[0] + coeficientes[1]) / 2;
     fxn = polinomio(coeficientes, grau, xn);
     delta = (xn - aux_xn) < 0 ? (xn - aux_xn) * -1 : (xn - aux_xn);
      
      if((fxn * aux_fn) < 0) {
        intervalo[0] = xn;
      }else {
        intervalo[1] = xn;
      }

      aux_xn = xn;
      aux_fn = fxn;

     printf("Interacao %d\n", i);
     printf("[%lf, %lf]\n", intervalo[0], intervalo[1]);
     printf("xn: %.7lf\n", xn);
     printf("fxn: %.7lf\n", fxn);
     printf("delta: %.7lf\n", delta);

    }
      
     i++;
     printf("\n");
   }

   return xn;
}

int main(){
     
   int grau;
   double coeficientes[20], intervalo[2], erro, raiz;

   printf("Informe o grau do polinomio: ");
   scanf("%d", &grau);
   printf("\n");  

   // infinito!!
   for(int i = grau; i >=0; i--) {  
      printf("Informe o coeficiente para x^%d: ", i);
      scanf("%lf", &coeficientes[i]);
   }

   printf("\n");

   for(int i = 0 ; i < 2; i++) {
      printf("Informe o intervalo inicial (a, b): ");
      scanf("%lf", &intervalo[i]);
   }
   
   if(polinomio(coeficientes, grau, intervalo[0]) * polinomio(coeficientes, grau, intervalo[1]) > 0) {
     printf("O intervalo nao contem uma raiz real.\n");
     return 0;
   }

   printf("\n");

   printf("Erro: ");
   scanf("%lf", &erro);

   clock_t start, end;
   double cpu_time_used;

   start = clock();

   raiz = bisseccao(grau, coeficientes, intervalo, erro);
   
   end = clock();
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   
   printf("Raiz: %.7lf\n", raiz);
   printf("Tempo de execução: %f segundos\n", cpu_time_used);


   return 0;
}