#include <stdio.h>
#include <time.h>

void L(double *x, int N, double *LK, double valor);
double polinomio(double *y, double *LK, int N);
void printPolinomio(double *x, double *LK, int N);
void print(double *x, double *y, double *LK, double result, int N);

int main() {
    int N;
    double valor, result;

    printf("Quantos pontos voce vai utilizar?\n");
    scanf("%d", &N);

    double x[N];
    double y[N];
    double LK[N];

    printf("Qual ponto voce quer interpolar?: \n");
    scanf("%lf", &valor);

    printf("Digite os valores das abscissas(X)\n");
    for (int i = 0; i < N; i++) {
        printf("\nX[%i]: ", i);
        scanf("%lf", &x[i]);
    }

    printf("Digite os valores das ordenadas(Y)\n");
    for (int i = 0; i < N; i++) {
        printf("\nY[%i]: ", i);
        scanf("%lf", &y[i]);
    }

    clock_t start_time = clock(); // Registrar o tempo de inícioss

    L(x, N, LK, valor);          // Calcula os coeficiente de Lagrange no ponto
    result = polinomio(y, LK, N); // Calcula o resultado do polinômio de Lagrange
    
    clock_t end_time = clock();

    printPolinomio(x, LK, N); // Printa o polinomio interpolador
    print(x, y, LK, result, N);
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %0.9f segundos\n", execution_time);

    return 0;
}

// Declaração das funções
// Calcula o coeficiente de Lagrange
void L(double *x, int N, double *LK, double valor) {

    for (int j = 0; j < N; j++) {
        double aux[2] = {1, 1};

        for (int i = 0; i < N; i++) {
            if (i != j) {
                aux[0] = (valor - x[i]) * aux[0];
                aux[1] = (x[j] - x[i]) * aux[1];
            }
        }
        LK[j] = (aux[0] / aux[1]);
    }
}

// Calcula o polinomio de Langrange
double polinomio(double *y, double *LK, int N) {
    double P = 0;
    for (int i = 0; i < N; i++) {
        P = P + (y[i] * LK[i]);
    }
    return P;
}

// Calcula e imprime o polinômio interpolador
void printPolinomio(double *x, double *LK, int N) {
    printf("Polinomio Interpolador:\n");
    
    printf("Pn(x) = ");
    for (int i = 0; i < N; i++) {
        if (i == 0) {
            printf("%.2lf", LK[i]);
        } else {
            if (LK[i] >= 0) {
                printf(" + %.2lf", LK[i]);
            } else {
                printf(" - %.2lf", -LK[i]);
            }

            for (int j = 0; j < i; j++) {
                if (x[j] >= 0) {
                    printf("*(x - %.2lf)", x[j]);
                } else {
                    printf("*(x + %.2lf)", -x[j]);
                }
            }
        }
    }

    printf("\n");
}

// Printa na tela todos os valores
void print(double *x, double *y, double *LK, double result, int N) {
    printf("--------------------------------------------------------\n\n");
    printf("Os pontos utilizados foram:\n");

    for (int i = 0; i < N; i++) {
        printf("X%d = %.2lf", i, x[i]);
        printf("           Y%d = %.2lf\n\n", i, y[i]);
    }
    printf("Os coeficientes de Lagrange sao:\n");

    for (int i = 0; i < N; i++) {
        printf("L%d = %.2lf\n\n", i, LK[i]);
    }

    printf("\n");
    printf("O valor de Y no ponto desejado: %.7lf\n", result);
}
