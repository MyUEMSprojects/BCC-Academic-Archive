#include "avl.h"

struct s_arvore *initTree()
{
    struct s_arvore *T = (struct s_arvore *)malloc(sizeof(struct s_arvore));
    T->raiz = NULL;
    return T;
}

struct s_no *newTreeNode(int32_t key)
{
    struct s_no *node = (struct s_no *)malloc(sizeof(struct s_no));
    node->chave = key;
    node->esq = NULL;
    node->dir = NULL;
    node->bal = 0;
    return node;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int getHeight(struct s_no *no)
{
    if (no == NULL)
        return 0;
    else
        return 1 + max(getHeight(no->esq), getHeight(no->dir));
}

void calcBal(struct s_no *no)
{
    if (no != NULL)
    {
        calcBal(no->esq);
        calcBal(no->dir);
        no->bal = getHeight(no->dir) - getHeight(no->esq);
    }
}

void case1(struct s_no **pt, bool *h)
{
    struct s_no *ptu = (*pt)->esq;
    if (ptu->bal == -1) // Rotação simples para a direita
    {
        (*pt)->esq = ptu->dir;
        ptu->dir = (*pt);
        (*pt)->bal = 0;
        (*pt) = ptu;
    }
    else // Rotação dupla direita
    {
        struct s_no *ptv = ptu->dir;
        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = (*pt);

        if (ptv->bal == -1)
            (*pt)->bal = 1;
        else
            (*pt)->bal = 0;

        if (ptv->bal == 1)
            ptu->bal = -1;
        else
            ptu->bal = 0;

        (*pt) = ptv;
    }
    (*pt)->bal = 0;
    *h = false;
}

void case2(struct s_no **pt, bool *h)
{
    struct s_no *ptu = (*pt)->dir;
    if (ptu->bal == 1) // Rotação simples para a esquerda
    {
        (*pt)->dir = ptu->esq;
        ptu->esq = (*pt);
        (*pt)->bal = 0;
        (*pt) = ptu;
    }
    else // Rotação dupla esquerda
    {
        struct s_no *ptv = ptu->esq;
        ptu->esq = ptv->dir;
        ptv->dir = ptu;
        (*pt)->dir = ptv->esq;
        ptv->esq = (*pt);

        if (ptv->bal == 1)
            (*pt)->bal = -1;
        else
            (*pt)->bal = 0;

        if (ptv->bal == -1)
            ptu->bal = 1;
        else
            ptu->bal = 0;

        (*pt) = ptv;
    }

    (*pt)->bal = 0;
    *h = false;
}

void insert(struct s_no **T, int32_t key, bool *h)
{
    if (*T == NULL)
    {
        *T = newTreeNode(key);
        *h = true;
        return;
    }
    else if (key == (*T)->chave)
    {
        return;
    }

    if (key < (*T)->chave)
    {
        insert(&(*T)->esq, key, h);
        if (*h)
        {
            switch ((*T)->bal)
            {
            case 1:
                (*T)->bal = 0;
                *h = false;
                break;
            case 0:
                (*T)->bal = -1;
                break;
            case -1:
                case1(T, h);
                break;
            }
        }
    }
    else
    {
        insert(&(*T)->dir, key, h);
        if (*h)
        {
            switch ((*T)->bal)
            {
            case -1:
                (*T)->bal = 0;
                *h = false;
                break;
            case 0:
                (*T)->bal = 1;
                break;
            case 1:
                case2(T, h);
                break;
            }
        }
    }
}

void transplant(struct s_no **root, struct s_no **u, struct s_no *v)
{
    if (*root == *u)
    {
        *root = v;
    }
    else
    {
        struct s_no **parent = root;
        while (*parent != NULL)
        {
            if ((*u)->chave < (*parent)->chave)
            {
                if ((*parent)->esq == *u)
                {
                    (*parent)->esq = v;
                    break;
                }
                parent = &(*parent)->esq;
            }
            else
            {
                if ((*parent)->dir == *u)
                {
                    (*parent)->dir = v;
                    break;
                }
                parent = &(*parent)->dir;
            }
        }
    }
}

struct s_no *tree_minimum(struct s_no *x)
{
    while (x->esq != NULL)
        x = x->esq;

    return x;
}

void tree_delete(struct s_no **root, struct s_no *z, bool *h)
{
    if (z == NULL || *root == NULL)
    {
        printf("\n\tNao há nada a ser removido\n");
        return;
    }

    if (z->esq == NULL)
    {
        transplant(root, &z, z->dir);
    }
    else if (z->dir == NULL)
    {
        transplant(root, &z, z->esq);
    }
    else
    {
        struct s_no *y = tree_minimum(z->dir);
        if (y != z->dir)
        {
            transplant(root, &y, y->dir);
            y->dir = z->dir;
        }
        transplant(root, &z, y);
        y->esq = z->esq;
    }

    // Ajustando o balanceamento após a remoção
    struct s_no **parent = root;
    while (*parent != NULL)
    {
        if (z->chave < (*parent)->chave)
        {
            (*parent)->bal++;
            if ((*parent)->bal == 1)
                break;
            else if ((*parent)->bal == 1 || (*parent)->bal == -1)
            {
                if ((*parent)->dir->bal >= 0)
                {
                    case2(parent, h); // Rotação para a esquerda
                }
                else
                {
                    case1(parent, h); // Rotação dupla esquerda
                }
            }
        }
        else
        {
            (*parent)->bal--;
            if ((*parent)->bal == -1)
                break;
            else if ((*parent)->bal == -2)
            {
                if ((*parent)->esq->bal <= 0)
                {
                    case1(parent, h); // Rotação para a direita
                }
                else
                {
                    case2(parent, h); // Rotação dupla direita
                }
            }
        }
        parent = (*parent)->chave > z->chave ? &(*parent)->esq : &(*parent)->dir;
    }

    free(z); // Liberando memória do nó removido
}

struct s_no *search(struct s_no *T, int32_t key)
{
    if (T == NULL)
        return NULL;
    else if (T->chave == key)
        return T;
    else
    {
        struct s_no *resultadoEsq = search(T->esq, key);
        if (resultadoEsq != NULL)
            return resultadoEsq;
        return search(T->dir, key);
    }
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

struct s_arvore *ReadFile(const char *filename)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "bin/%s", filename);

    FILE *file = fopen(filepath, "rb");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo para leitura");
        return NULL;
    }

    struct s_arvore *T = initTree();
    T->raiz = readNode(file);

    fclose(file);

    return T;
}

void deallocateTree(struct s_no *T)
{
    if (T == NULL)
        return;
    deallocateTree(T->esq);
    deallocateTree(T->dir);
    free(T);
}