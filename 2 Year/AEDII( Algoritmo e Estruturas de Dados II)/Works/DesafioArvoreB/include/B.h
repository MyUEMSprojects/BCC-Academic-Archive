#ifndef B_H
#define B_H

#include "globalVariables.h"

struct NoB
{
    int num_chaves;                // Número de chaves no nó
    int chaves[2 * D];             // Vetor de chaves, pode armazenar até 2*ORDEM chaves
    struct NoB *filhos[2 * D + 1]; // Vetor de ponteiros para os filhos (2*ORDEM+1 filhos)
    int eh_folha; // Flag que indica se o nó é uma folha (1) ou não (0)
};

struct ArvoreB
{
    struct NoB *raiz; // Ponteiro para o nó raiz
};

void inserir(struct NoB* no, int32_t x);
void remover(struct NoB* no, int32_t x);

#endif /* GLOBAL_VARIABLES_H*/