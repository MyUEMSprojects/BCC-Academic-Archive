#ifndef AVL_H
#define AVL_H

#include "globalVariables.h"

// nó da arvore
struct s_no
{
    int32_t chave : 28;
    int32_t bal : 2;
    int32_t reservado : 2; /* sem uso */
    struct s_no *esq;
    struct s_no *dir;
};

// struct da arvore com um ponteiro do tipo s_no com a raiz da arvore
struct s_arvore
{
    struct s_no *raiz;
};

// struct para lidar com a arvore em arquivo binário
struct s_arq_no
{
    int32_t chave : 28;
    int32_t bal : 2;
    uint32_t esq : 1;
    uint32_t dir : 1;
};

// Busca, inserção e remoção de chaves inteiras: quando necessário,
// devem ser realizadas as rotações adequadas

// inicializa a arvore
struct s_arvore *initTree();

// criar novo nó na arvore
struct s_no *newTreeNode(int32_t key);

// função auxiliar para calcular a altura da arvore
int max(int a, int b);

// função para calcular a altura da arvore
int getHeight(struct s_no* no);

// função para calcular e preencher o campo de balanceamento da arvore
void calcBal(struct s_no* no);

// função para realizar a rotação para a direita ou dupla direita
void case1(struct s_no** pt, bool* h);

// função para realizar a rotação para esquerda ou dupla esquerda
void case2(struct s_no** pt, bool* h);

// procedimento para inserir uma chave na AVL
void insert(struct s_no** T, int32_t key, bool *h);

// função auxiliar Transplant
// * substitui a subárvore com raiz em u pela subárvore com raiz em v
void transplant(struct s_no **root, struct s_no **u, struct s_no *v);

// função para encontrar o menor nó em uma subárvore
struct s_no* tree_minimum(struct s_no* x);
/*
- procedimento para deletar um nó em uma avl
   * Aplicar a remoção de arvore binaria de busca
   * verificar se a arvore ficou desregulada
     * analisar nós no caminho até a raiz
     * podem ser utilizados até O(lg n) rotações
*/
void tree_delete(struct s_no **root, struct s_no *z, bool *h);

// procedimento de busca na AVL
struct s_no* search(struct s_no* T, int32_t key);

// - Escrever a arvore binária no arquivo binário
// - Leitura e gravação da árvore em arquivo formato binário usando percurso em pré-ordem. A árvore não deve ser reconstruída através de operações de inserção;
void writeNode(FILE *file, struct s_no *node);
// - Escrever a arvore binária no arquivo binário
// - Leitura e gravação da árvore em arquivo formato binário usando percurso em pré-ordem. A árvore não deve ser reconstruída através de operações de inserção;
void WriteFile(struct s_no *T, const char *filename);

// ler a arvore binária do arquivo binário
struct s_no *readNode(FILE *file);
// ler a arvore binária do arquivo binário
struct s_arvore *ReadFile(const char *filename);

// desalocação da arvore binaria
void deallocateTree(struct s_no *T);

#endif /*AVL_H*/