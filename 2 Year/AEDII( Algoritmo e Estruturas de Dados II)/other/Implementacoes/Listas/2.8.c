#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M 100

struct No
{
    int chave;
    struct No* prox;
};

struct No* aloca(int chave);
// inserção em uma lista circular simplesmente encadeada
void inserir(struct No** L, int chave);
void remover(struct No** L);
void imprimir(struct No* L);
void desaloca(struct No** L);

int main()
{

    struct No* L = NULL;
    inserir(&L, 1);
    inserir(&L, 2);
    inserir(&L, 3);
    inserir(&L, 4);
    remover(&L);
    imprimir(L);
    
    desaloca(&L);
    return 0;
}

struct No *aloca(int chave)
{
    struct No* novo_No = (struct No*) malloc(sizeof(struct No));
    novo_No->chave = chave;
    return novo_No;
}

void inserir(struct No **L, int chave)
{

    struct No* novo = aloca(chave);
    if(*L == NULL)
    {
        novo->prox = novo;
        *L = novo;
    }
    else
    {
        struct No* pont = *L;
        while (pont->prox != *L)
        {
            pont = pont->prox;
        }

        novo->prox = *L;
        pont->prox = novo;
    }
}

void remover(struct No **L)
{
    if(*L == NULL)
    {
        return;
    }
    else
    {
        struct No* pont = *L;
        while (pont->prox->prox != *L)
        {
            pont = pont->prox;
        }
        struct No* remove = pont->prox;
        pont->prox = remove->prox; // remove->prox == *L
        free(remove);
    }
}

void imprimir(struct No* L)
{
    if(L == NULL)
    {
        printf("\nLista vazia!");
        return;
    }

    struct No *pont = L;
    do
    {
        printf("%d ", pont->chave);
        pont = pont->prox;
    } while (pont != L);
    printf("\n");
}

void desaloca(struct No **L)
{
    if(*L == NULL)
    {
        return;
    }

    struct No* pont = (*L)->prox;
    struct No* tmp;

    while (pont != NULL && pont != *L)
    {
        tmp = pont;
        pont = pont->prox;
        free(tmp);
    }
    
    free(*L);

    *L = NULL;
}