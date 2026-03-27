/* Inversão de Matrizes pelo método de Jordan

 Entrada:
    - ordem da matriz (Máximo 10)
    - Matriz

 Saída:
   - Resultados de cada passo com 1 casa decimal
   - Matriz inversa
   - Tempo de execução
 */

/* Resolução de sistemas lineares pelo método de Jacobi

Entrada:
  - ordem da matriz (máximo 10) dos coeficientes das incógnitas
  - Sistema linear
  - Precisão
  - Solução inicial
Saída:
  - Resultados de cada passo (sistema e erro) com 7 casas decimais
  - Solução
  - Tempo de execução
*/

#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAX_SIZE 10

void jordan(){

    int n, i, j, k;
    double a[MAX_SIZE][MAX_SIZE*2], c, t, s;
    clock_t inicio, fim;

   do {
       printf("Digite a ordem da matriz (no maximo %d): ", MAX_SIZE);
       scanf("%d", &n);
   } while (n < 1 || n > 10);


    printf("Digite a matriz:\n");
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
            scanf("%lf", &a[i][j]);
        }
    }

    // Acrescenta a matriz identidade à direita(do lado) da matriz original
    for(i=0; i<n; i++) {
        for(j=n; j<n*2; j++) {
            if(j-n == i) {
                a[i][j] = 1.0;
            } else {
                a[i][j] = 0.0;
            }
        }
    }

    inicio = clock();

    // Faz a eliminação de Gauss-Jordan
    for(k=0; k<n; k++) {
        c = a[k][k];
        for(j=k; j<n*2; j++) {
            a[k][j] /= c;
        }
        for(i=0; i<n; i++) {
            if(i != k) {
                t = a[i][k];
                for(j=k; j<n*2; j++) {
                    a[i][j] -= t * a[k][j];
                }
            }
        }
    }

    

    // Extrai a matriz inversa da matriz resultante
    printf("Matriz inversa:\n");
    for(i=0; i<n; i++) {
        for(j=n; j<n*2; j++) {
            printf("%0.1lf ", a[i][j]);
        }
        printf("\n");
    }

    fim = clock();

    // Imprime o tempo de execução
    double tempo_exec = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %0.9lf segundos\n", tempo_exec);
}

// resolução jacobi
void jacobi() {

    int n, i, j, k;
    double a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE], x[MAX_SIZE], xo[MAX_SIZE], eps, erro, t;
    clock_t inicio, fim;

    do {
        printf("Digite a ordem da matriz (no maximo %d): ", MAX_SIZE);
        scanf("%d", &n);
    } while (n < 1 || n > 10);

    printf("Digite a matriz dos coeficientes:\n");
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
            scanf("%lf", &a[i][j]);
        }
    }

    printf("Digite o vetor das constantes:\n");
    for(i=0; i<n; i++) {
        scanf("%lf", &b[i]);
    }

    printf("Digite a precisao desejada: ");
    scanf("%lf", &eps);

    printf("Digite a solucao inicial:\n");
    for(i=0; i<n; i++) {
        scanf("%lf", &xo[i]);
    }

    // Verificação das linhas
    for(i=0; i<n; i++) {
        double soma = 0.0;
        for(j=0; j<n; j++) {
            if(i != j) {
                soma += fabs(a[i][j]);
            }
        }
        if(fabs(a[i][i]) <= soma) {
            printf("A matriz fornecida nao satisfaz a condicao de linhas. O metodo de Jacobi pode nao convergir.\n");
            return;
        }
    }

    inicio = clock();

    k = 0;
    do {
        erro = 0;
        for(i=0; i<n; i++) {
            x[i] = b[i];
            for(j=0; j<n; j++) {
                if(i != j) {
                    x[i] -= a[i][j] * xo[j];
                }
            }
            x[i] /= a[i][i];
            erro += fabs(x[i] - xo[i]);
        }
        k++;
        printf("Passo %d: x = [", k);
        for(i=0; i<n; i++) {
            printf("%0.7lf ", x[i]);
        }
        printf("] erro = %0.10lf\n", erro);
        for(i=0; i<n; i++) {
            xo[i] = x[i];
        }
    } while(erro > eps);

    fim = clock();

    printf("Solucao encontrada: [");
    for(i=0; i<n; i++) {
        printf("%0.7lf ", x[i]);
    }
    printf("]\n");

    t = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %0.9lf segundos\n", t);

}

int main() {

    int opc = 0;

    do {
        printf("\n\t1 - Jordan\n\t2 - Jacobi\n");
        scanf("%d", &opc);
    }while(opc != 1 && opc != 2);

    if(opc == 1) {
        jordan();
    }else {
        jacobi();
    }
    return 0;
}