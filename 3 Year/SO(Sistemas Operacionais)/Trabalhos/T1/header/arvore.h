#ifndef ARVORE_H
#define ARVORE_H

/*
 * header dedicado a organização da arvore de processos
 * */

#include <libs.h>

struct Node
{
    char name[50]; // nome do processo
    int pid;         // pid
    int num_child;   // quantidade de filhos
    struct Node **children; // filhos
};

// Funcao para criar um no com PID pid e NOME name
struct Node *createNode(int pid, char *name);

// busca um no recursivamente pelo PID partindo de n
struct Node *findNode(struct Node *node, int pid);

// desalocar arvore
void freeTree(struct Node *node);

// printa a arvore recursivamente
void printTree(struct Node *node, int level);

// Funcao para adicionar um no child para um no parent
int addChild(struct Node *parent, struct Node *child);

void buildTree(int pid, struct Node **root);

// verifica se uma string é um digito, ou seja, uma string no formato numerico
static int isNumeric(const char *str);

#endif
