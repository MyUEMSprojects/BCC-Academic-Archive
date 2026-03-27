#ifndef INTERFACE_H
#define INTERFACE_H

#include "avl.h"

// Visualização da árvore em modo gráfico usando a biblioteca gfx. 
//Deve ser impressa a representação hierárquica contendo  a chave e o balanço de cada nó;

// calcular posição X do nó
int calcularPosX(struct s_no *L, int level, int horizontalSpacing);

// Função para desenhar uma conexão entre dois nós da árvore
void drawConection(int x1, int y1, int x2, int y2);

// Função para desenhar um nó da árvore
void drawNode(int x, int y, int key, int balance);

// Função para desenhar a árvore
void drawTree(struct s_no *no, int level, int posX, int posY, int horizontalSpacing, int verticalSpacing);

// atualiza o display
void updateTreeDisplay(struct s_arvore *tree);

#endif /*INTERFACE_H*/