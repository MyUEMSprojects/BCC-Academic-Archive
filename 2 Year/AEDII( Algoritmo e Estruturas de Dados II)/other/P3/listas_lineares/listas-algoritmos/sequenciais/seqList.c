#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

struct List {
   int elements[MAX_SIZE];
   int c;
};

void append(struct List* list, int data, char E[10]) {
    if(list->c >= MAX_SIZE) {
        printf("Lista cheia\n");
        return;
    }

    if(list->c <= 0 || strcmp(E, "E1") == 0) {
        for(int i = list->c; i > 0; i--) {
            list->elements[i] = list->elements[i - 1];
        }
        list->elements[0] = data;
        list->c++;
        return;
    }

    if(strcmp(E, "E2") == 0) {
        list->elements[list->c] = data;
        list->c++;
        return;
    }
}

void remover(struct List* list, char E[10]) {
    if(list->c <= 0) {
        printf("lista vazia\n");
        return;
    }

    if(strcmp(E, "E1") == 0) {
        for(int i = 0; i < list->c - 1; i++) {
            list->elements[i] = list->elements[i + 1];
        }
        list->c--;
        return;
    }

    if(strcmp(E, "E2") == 0) {
        list->elements[list->c - 1] = 0;
        list->c--;
        return;
    }
}

void display(struct List* list) {
    for(int i = 0; i < list->c; i++) {
        printf("%d ", list->elements[i]);
    }
    printf("\n");
}

int main() {

    struct List list;
    list.c = 0;

    append(&list, 1, "E1");
    display(&list);

    append(&list, 2, "E2");
    display(&list);

    remover(&list, "E1");
    display(&list);

    remover(&list, "E2");
    display(&list);

    return 0;
}