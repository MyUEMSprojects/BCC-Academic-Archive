#include <stdio.h>
#include <math.h>
#include <time.h>

double polinomio(int grau, double coeficientes[], double x) {
   double operacao = 0.0;  
   for (int i = 0; i <= grau; i++) {
     operacao += coeficientes[i] * pow(x, i);
   }
   return operacao;
}

void algoritmo(int grau, double coeficientes[], double intervalo[]) {
   double epsilon = 1e-6;
   double novo_intervalo = intervalo[1] - intervalo[0];

   while (novo_intervalo > epsilon) {
      double meio = (intervalo[0] + intervalo[1]) / 2;
      double valorMeio = polinomio(grau, coeficientes, meio);

      if (fabs(valorMeio) < epsilon) {
         printf("Raiz encontrada no intervalo: [%lf, %lf]\n", intervalo[0], intervalo[1]);
         return;
      }

      if (valorMeio * polinomio(grau, coeficientes, intervalo[0]) < 0) {
         intervalo[1] = meio;
      } else {
         intervalo[0] = meio;
      }

      novo_intervalo = intervalo[1] - intervalo[0];
   }

   printf("Nao foi possivel encontrar a raiz.\n");
}

int main() {
   int grau;
   double coeficientes[20], intervalo[2];

   printf("Informe o grau do polinomio: ");
   scanf("%d", &grau);
   printf("\n");  

   for (int i = grau; i >= 0; i--) {
      printf("Informe o coeficiente para x^%d: ", i);
      scanf("%lf", &coeficientes[i]);
   }

   printf("\n");

   printf("Informe o intervalo inicial (a, b): ");
   scanf("%lf %lf", &intervalo[0], &intervalo[1]);

   // Verifica se o intervalo inicial contém uma raiz
   if (polinomio(grau, coeficientes, intervalo[0]) * polinomio(grau, coeficientes, intervalo[1]) > 0) {
      printf("O intervalo nao contem uma raiz real.\n");
      return 0;
   }

   clock_t start, end;
   double cpu_time_used;

   start = clock();

   algoritmo(grau, coeficientes, intervalo);

   end = clock();
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   
   printf("Tempo de execução: %f segundos\n", cpu_time_used);

   return 0;
}