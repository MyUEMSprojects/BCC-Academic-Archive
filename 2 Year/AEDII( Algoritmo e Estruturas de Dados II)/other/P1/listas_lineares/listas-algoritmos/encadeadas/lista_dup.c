#include <stdio.h>
#include <stdlib.h>

// Implmentação de uma lista duplamente encadeada circular com nó cabeça

// Definição da estrutura do nó
struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

// Função para criar um novo nó
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

struct DoublyLinkedList {
    struct Node* head; // Nó cabeça
};

// Função para inicializar a lista
void initializeList(struct DoublyLinkedList* list) {
    list->head = createNode(-1); // Criando o nó cabeça com valor -1
    list->head->next = list->head;
    list->head->prev = list->head;
}

// Função para adicionar um elemento no final da lista
void append_final(struct DoublyLinkedList* list, int data) {
    struct Node* newNode = createNode(data);
    newNode->prev = list->head->prev;
    newNode->next = list->head;
    list->head->prev->next = newNode;
    list->head->prev = newNode;
}

// Função para adicionar um elemento no final da lista
void append_front(struct DoublyLinkedList* list, int data) {
    struct Node* newNode = createNode(data);
    newNode->prev = list->head;
    newNode->next = list->head->next;
    list->head->next->prev = newNode;
    list->head->next = newNode;
}

// remove do final
void pop_final(struct DoublyLinkedList* list) {
        if(list == NULL) {
            return;
        }
        
        list->head->prev->prev->next = list->head;
        struct Node* aux = list->head->prev;
        list->head->prev = list->head->prev->prev;
        free(aux);
}

// remove do inicio
void pop_front(struct DoublyLinkedList* list) {
    if(list == NULL || list->head->next == list->head) {
        return;
    }
    
    list->head->next->prev = list->head->prev;
    list->head->prev->next = list->head->next;

    struct Node* aux = list->head;

    list->head = list->head->next;

    free(aux);
}

void deallocateMemory(struct DoublyLinkedList* list) {
    if(list == NULL) {
        return;
    }
    
    struct Node* current = list->head->next;
    
    // libera os nós 
    while(current != list->head) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    
    // libera o nó cabeça
    free(list->head);

    list->head = NULL;
}

// Função para exibir os elementos da lista
void display(struct DoublyLinkedList* list) {
    struct Node* current = list->head->next;
    printf("Elementos da lista: ");
    while (current != list->head) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    struct DoublyLinkedList list;
    struct DoublyLinkedList list2;

    // Inicializa a lista
    initializeList(&list);
    initializeList(&list2);

    // Adiciona alguns elementos à lista
    append_final(&list, 1);
    append_final(&list, 2);
    append_final(&list, 3);
    pop_final(&list);
    pop_front(&list);
    // Exibe os elementos da lista
    display(&list);

    printf("\n");

    append_front(&list2, 1);
    append_front(&list2, 2);
    append_front(&list2, 3);


    display(&list2);

    deallocateMemory(&list);

    return 0;
}