#include <stdio.h>
#include <stdlib.h>

struct No {
   struct No *prox;
   int chave; 
};

struct No* criarNo(int chave) {
   struct No* novoNo = (struct No*) malloc(sizeof(struct No));
   novoNo->chave = chave;
   novoNo->prox = NULL;
   return novoNo;
}

struct No* inserir_inicio(struct No *ptLista, int chave) {
    struct No* novoNo = criarNo(chave);
    if(ptLista == NULL) {
       ptLista = novoNo;
    }else {
        novoNo->prox = ptLista;
        ptLista = novoNo;
    }

    return ptLista;
}

struct No* inserir_final(struct No *ptLista, int chave) {
    struct No* novoNo = criarNo(chave);
    
    if(ptLista == NULL) {
        return novoNo;
    }

    struct No* temp = ptLista;
    
    while(temp->prox != NULL) {
        temp = temp->prox;
    }

    temp->prox = novoNo;

    return ptLista;
}

struct No* remover_inicio(struct No *ptLista) {
    if(ptLista == NULL) return NULL;

    struct No* prox = ptLista->prox;
    free(ptLista);

    return prox;
}

struct No* remover_final(struct No *ptLista) {
    if(ptLista == NULL) return NULL;

   if(ptLista->prox == NULL){
      free(ptLista);
      return NULL;
   }

   struct No* temp = ptLista;
   struct No* ant = NULL;

   while(temp->prox != NULL) {
    ant = temp;
    temp = temp->prox;
   }

   if(ant != NULL) {
     ant->prox = NULL;
   }

   free(temp);

   return ptLista;
}

struct No* desaloca(struct No* ptLista) {
    struct No* temp = ptLista;

    while(ptLista != NULL) {
        temp = ptLista->prox;
        free(ptLista);
        ptLista = temp;
    }

    return NULL;
}

void buscaChave(struct No* ptLista, int x) {
    struct No *temp = ptLista;

    while(temp != NULL) {
        if(temp->chave == x) {
            printf("\nChave encontrada! %d", temp->chave);
            temp = NULL;
        }else if(temp->prox == NULL){
            printf("\nChave nao encontrada!");
            temp = NULL;
        }else {
            temp = temp->prox;
        }
    }

}

struct No* juncao(struct No* A, struct No* B) {
    struct No* tempA = A;
    struct No* tempB = B;
    struct No* C = NULL;
    
    if(A == NULL && B == NULL) {
        return NULL;
    }

    if(A == NULL) {
        return B;
    }

    if(B == NULL) {
        return A;
    }

    while(tempA != NULL) {
        while(tempB != NULL) {
            if(tempA->chave == tempB->chave) {
                C = inserir_inicio(C, tempA->chave);             
            }
            tempB =  tempB->prox;
        }
        tempB = B;
        
        tempA = tempA->prox;
    }

    return C; 
}

void imprimir(struct No* ptLista) {
    struct No *temp = ptLista;

    while(temp != NULL) {
        printf("%d\n", temp->chave);
        temp = temp->prox;
    }
}

int main() {
   
    struct No *lista = NULL;
    lista = inserir_inicio(lista, 1);
    lista = inserir_inicio(lista, 2);
    lista = inserir_inicio(lista, 3);
    lista = inserir_final(lista, 4);

    struct No *lista2 = NULL;
    lista2 = inserir_inicio(lista2, 1);
    lista2 = inserir_inicio(lista2, 2); 
    lista2 = inserir_inicio(lista2, 3); 
    
    struct No* lista3 = NULL;
    lista3 = juncao(lista, lista2);
    imprimir(lista3);
        
    lista = desaloca(lista);
    lista2 = desaloca(lista2);
    return 0;
}