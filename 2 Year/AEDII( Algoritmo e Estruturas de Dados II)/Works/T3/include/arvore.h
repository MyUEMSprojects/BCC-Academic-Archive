#ifndef ARVORE_H
#define ARVORE_H
#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <stdint.h> /* int32_t */

// nó da arvore
struct s_no
{
    int32_t chave;
    struct s_no *esq;
    struct s_no *dir;
};

// struct da arvore com um ponteiro do tipo s_no com a raiz da arvore
struct s_arvore
{
    struct s_no *raiz;
};

// nó do arquivo binario
struct s_arq_no
{
    int32_t chave : 30;
    uint32_t esq : 1;
    uint32_t dir : 1;
};

struct s_arvore *initTree();

// criar novo nó na arvore
struct s_no *newTreeNode(int32_t key);
// busca pelo maior elemento da arvore
struct s_no *searchLarger(struct s_no *root);
// busca pelo menor elemento da arvore
struct s_no *searchSmaller(struct s_no *root);
// busca por uma chave(key) x na arvore
struct s_no *search(struct s_no *root, int32_t key);
// busca pelo sucessor de uma chave(key) com valor x
struct s_no *searchSuccessor(struct s_no *root, int32_t key);
// busca pelo predecessor de um nó com uma chave(key)
struct s_no *searchPredecessor(struct s_no *root, int32_t key);
// inserção de uma nó na arvore com uma chave(key)
void insert(struct s_arvore *T, int32_t key);
// remoção de uma chave(key) com valor x na arvore
struct s_no *delete(struct s_no *T, int32_t key);

// - Escrever a arvore binária no arquivo binário
// - Leitura e gravação da árvore em arquivo formato binário usando percurso em pré-ordem. A árvore não deve ser reconstruída através de operações de inserção;
void writeNode(FILE *file, struct s_no *node);
// - Escrever a arvore binária no arquivo binário
// - Leitura e gravação da árvore em arquivo formato binário usando percurso em pré-ordem. A árvore não deve ser reconstruída através de operações de inserção;
void WriteFile(struct s_no *T, const char *filename);

// ler a arvore binária do arquivo binário
struct s_no *readNode(FILE *file);
// ler a arvore binária do arquivo binário
void readFile(struct s_arvore **T, const char *filename);

// desalocação da arvore binaria
void deallocateTree(struct s_no *T);

#endif