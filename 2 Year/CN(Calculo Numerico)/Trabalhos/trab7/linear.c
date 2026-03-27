#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Estrutura para representar um ponto (x, y)
typedef struct {
    double x;
    double y;
} Point;

// Função para calcular a regressão linear
void linearRegression(Point* points, int numPoints, double* a, double* b) {
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;
    
    for (int i = 0; i < numPoints; i++) {
        sumX += points[i].x;
        sumY += points[i].y;
        sumXY += points[i].x * points[i].y;
        sumX2 += points[i].x * points[i].x;
    }
    
    *a = (numPoints * sumXY - sumX * sumY) / (numPoints * sumX2 - sumX * sumX);
    *b = (sumY - *a * sumX) / numPoints;
}

int main() {
    int numPoints;
    printf("Digite o número de pontos: ");
    scanf("%d", &numPoints);

    // Aloca espaço para armazenar os pontos
    Point* points = (Point*)malloc(numPoints * sizeof(Point));

    // Lê os pontos de entrada
    printf("Digite os pontos (x y):\n");
    for (int i = 0; i < numPoints; i++) {
        scanf("%lf %lf", &points[i].x, &points[i].y);
    }

    clock_t start_time = clock(); // Mede o tempo de execução

    double a, b;
    linearRegression(points, numPoints, &a, &b);

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Equação da reta: y = %.6lf * x + %.6lf\n", a, b);
    printf("Tempo de execução: %.6lf segundos\n", execution_time);

    // Libera a memória alocada
    free(points);

    return 0;
}
