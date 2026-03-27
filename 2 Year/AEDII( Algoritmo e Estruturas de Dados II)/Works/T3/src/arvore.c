#include "arvore.h"

struct s_no *newTreeNode(int32_t key)
{
    struct s_no* node = (struct s_no* )malloc(sizeof(struct s_no));
    node->chave = key;
    node->esq = NULL;
    node->dir = NULL;
    return node;
}

struct s_arvore *initTree()
{
    struct s_arvore* T = (struct s_arvore*)malloc(sizeof(struct s_arvore));
    T->raiz = NULL;
    return T;
}

struct s_no* searchLarger(struct s_no *root)
{
    struct s_no* temp = root;

    while(temp->dir != NULL){
        temp = temp->dir;
    }

    return temp;
}

struct s_no *searchSmaller(struct s_no *root)
{
    struct s_no* temp = root;

    while(temp->esq != NULL){
        temp = temp->esq;
    }

    return temp;
}

struct s_no *search(struct s_no *root, int32_t key)
{
    if(root == NULL || key == root->chave) return root;

    if(key < root->chave) {
       return search(root->esq, key);
    }

    return search(root->dir, key);
}

struct s_no *searchSuccessor(struct s_no *root, int32_t key)
{
    struct s_no *node = search(root, key);
    if (node == NULL)
        return NULL;
    if (node->dir != NULL)
    {
        return searchSmaller(node->dir);
    }
    struct s_no *sucessor = NULL;
    struct s_no *ancestor = root;
    while (node->chave != ancestor->chave)
    {
        if (node->chave < ancestor->chave)
        {
            sucessor = ancestor;
            ancestor = ancestor->esq;
        }
        else
        {
            ancestor = ancestor->dir;
        }
    }
    return sucessor;
}

struct s_no *searchPredecessor(struct s_no *root, int32_t key)
{
    struct s_no* node = search(root, key);
    if(node == NULL) return NULL;

    if(node->esq != NULL) {
        return searchLarger(node->esq);
    }

    struct s_no* predecessor = NULL;
    struct s_no* ancestor = root;

    while(ancestor != node) {
        if(node->chave > ancestor->chave) {
            predecessor = ancestor;
            ancestor = ancestor->dir;
        } else {
            ancestor = ancestor->esq;
        }
    }

    return predecessor;
}

void insert(struct s_arvore *T, int32_t key)
{   
    struct s_no* z = newTreeNode(key);

    struct s_no *y = NULL;
    struct s_no *x = T->raiz;

    while (x != NULL)
    {
        y = x;

        if (z->chave < x->chave)
        {
            x = x->esq;
        }
        else
        {
            x = x->dir;
        }
    }

    if (y == NULL)
    {
        T->raiz = z;
    }
    else if (z->chave < y->chave)
    {
        y->esq = z;
    }
    else
    {
        y->dir = z;
    }
}

struct s_no *delete(struct s_no *root, int32_t key)
{
    if (root == NULL)
        return root;

    if (key < root->chave)
    {
        root->esq = delete (root->esq, key);
    }
    else if (key > root->chave)
    {
        root->dir = delete (root->dir, key);
    }
    else
    {
        if (root->esq == NULL)
        {
            struct s_no *temp = root->dir;
            free(root);
            return temp;
        }
        else if (root->dir == NULL)
        {
            struct s_no *temp = root->esq;
            free(root);
            return temp;
        }
        struct s_no *temp = searchSmaller(root->dir);
        root->chave = temp->chave;
        root->dir = delete (root->dir, temp->chave);
    }
    return root;
}

void writeNode(FILE *file, struct s_no *node)
{
    if (node == NULL)
        return;

    struct s_arq_no temp;
    temp.chave = node->chave & 0x3FFFFFFF;
    temp.esq = (node->esq != NULL) ? 1 : 0;
    temp.dir = (node->dir != NULL) ? 1 : 0;

    fwrite(&temp, sizeof(struct s_arq_no), 1, file);

    writeNode(file, node->esq);
    writeNode(file, node->dir);
}

void WriteFile(struct s_no *T, const char *filename)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "bin/%s", filename);

    FILE *file = fopen(filepath, "wb");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    writeNode(file, T);

    fclose(file);
}

struct s_no *readNode(FILE *file)
{
    struct s_arq_no temp;
    if (fread(&temp, sizeof(struct s_arq_no), 1, file) != 1)
        return NULL;

    struct s_no *node = newTreeNode(temp.chave);
    if (temp.esq)
    {
        node->esq = readNode(file);
    }
    if (temp.dir)
    {
        node->dir = readNode(file);
    }

    return node;
}

void readFile(struct s_arvore **T, const char *filename)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "bin/%s", filename);

    FILE *file = fopen(filepath, "rb");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo para leitura");
        return;
    }

    // Primeiro, desaloca a árvore atual
    deallocateTree((*T)->raiz);
    free(*T);

    // Inicializa uma nova árvore
    *T = initTree();
    (*T)->raiz = readNode(file);

    fclose(file);
}

void deallocateTree(struct s_no *T)
{
    if (T == NULL)
        return;
    deallocateTree(T->esq);
    deallocateTree(T->dir);
    free(T);
}