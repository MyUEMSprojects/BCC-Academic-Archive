#include <stdio.h>
#include <time.h>

int factorial(int n) {
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

int main() {
    int n;
    printf("Digite o número de pontos: ");
    scanf("%d", &n);
    float x, X[n], Y[n][n];
    printf("\nDigite os valores das abscissas(X)\n\n");
    for (int i = 0; i < n; i++) {
        printf("X[%i]: ", i);
        scanf("%f", &X[i]);
    }
    printf("\nDigite os valores das ordenadas(Y)\n\n");
    for (int i = 0; i < n; i++) {
        printf("Y[%i]: ", i);
        scanf("%f", &Y[i][0]);
    }
    printf("Qual ponto voce quer interpolar?: ");
    scanf("%f", &x);
     
    clock_t start_time = clock(); // Registrar o tempo de início
    
    // Calcular as diferenças divididas
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < n - i; j++) {
            Y[j][i] = (Y[j + 1][i - 1] - Y[j][i - 1]) / (X[j + i] - X[j]);
        }
    }

    clock_t end_time = clock(); // Registrar o tempo de fim

    // Imprimir a tabela das diferenças divididas
    printf("\nTabela das diferenças divididas:\n");
    for (int i = 0; i < n; i++) {
        printf("X%d = %0.2f", i, X[i]);
        for (int j = 0; j <= i; j++) {
            printf("\t%f", Y[i-j][j]);
        }
        printf("\n");
    }

    // Calcular o valor interpolado
    float result = Y[0][0];
    float term = 1.0;

    for (int i = 1; i < n; i++) {
        term *= (x - X[i - 1]);
        result += term * Y[0][i];
    }

    // Calcular o tempo de execução
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nValor interpolado de Y para X=%0.2f é %0.7f\n", x, result);
    printf("Tempo de execução: %0.9f segundos\n", execution_time);
    return 0;
}
