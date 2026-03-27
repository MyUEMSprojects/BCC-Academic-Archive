#include <stdio.h>

int main(void){
    
    float notas[4][4] = {0};
    float medialunos[4];
    float media = 0;
    int i,j;
    
    printf("Insira as 4 notas do aluno 1:\n");
    
    for (i = 0; i < 4; ++i){
        for (j = 0; j < 4; ++j){
            scanf("%f", &notas[i][j]);
            media += notas[i][j];
        }
        medialunos[i] = media / 4;
        media = 0;
        
        if(i + 1 < 4){
            printf("Insira as 4 notas do aluno %d:\n",i+2);
        }
    }
    
    for(i = 0; i < 4; i++){
        printf("A media do aluno %d, = %.2f\n",i + 1, medialunos[i]);
    }
    
    return 0;
}