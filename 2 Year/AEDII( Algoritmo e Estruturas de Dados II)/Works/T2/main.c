#include "gfx.h"
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep  */
#include <stdlib.h>

#define WIDTH 1270
#define HEIGHT 900

// voce pode aumentar ou diminuir as dimensoes e os espaçamentos, porém os nó podem na caber na tela
#define NODE_WIDTH 100
#define NODE_HEIGHT 50
#define NODE_SPACING 20

/*
Nome: Felipe Echeverria Vilhalva RGM:45611 Trabalho 2 de AEDII 2024
*/

// <Busca, inserção e remoção em lista simplesmente encadeada sem nó cabeça ordenada
struct SimpleNode {
    int data;
    struct SimpleNode* next;
};

// Função para criar um nó
struct SimpleNode*  createNode(int data) {
    struct SimpleNode* newNode = (struct SimpleNode* ) malloc(sizeof(struct SimpleNode));

    if(!newNode) {
        printf("Memory error\n");
    }

    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}


// Função para inserir um nó no inicio lista
void insertSimpleNode(struct SimpleNode** head, int data) {
    struct SimpleNode* newNode = createNode(data);
    if(!newNode) return;

    if(*head == NULL || (*head)->data >= data) {
        newNode->next = *head;
        *head = newNode;
    }else {
        struct SimpleNode* current = *head;
        while(current->next != NULL && current->next->data < data) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }
}

// Função para remover o primeiro nó da lista
void removeFirstNodeSimpleNode(struct SimpleNode** head) {
    if(*head==NULL) return;

    struct SimpleNode* temp = *head;
    *head = (*head)->next;
    free(temp);
}

// Função para procurar um nó na lista
struct SimpleNode* searchSimpleNode(struct SimpleNode* head, int data) {
    while(head != NULL &&  head->data < data) {
        head = head->next;
    }

    if(head != NULL && head->data == data) {
        return head;
    }

    return NULL;
}

// Função para imprimir a lista
void printSimpleList(struct SimpleNode* head) {
    struct SimpleNode* temp = head;
    while(temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;  
    }
}

// Função para desalocar(liberar) a lista
void freeSimpleList(struct SimpleNode** head) {
    struct SimpleNode* current = *head;
    struct SimpleNode* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
     
    *head = NULL;
}

// fim>

// <Busca, inserção e remoção em lista circular simplesmente encadeada sem nó cabeça ordenada

// Função para inserir um nó no inicio da lista circular
void insertSimpleNodeCircular(struct SimpleNode** tail, int data) {
    struct SimpleNode* newNode = (struct SimpleNode*)malloc(sizeof(struct SimpleNode));
    newNode->data = data;

    // Se a lista estiver vazia
    if (!*tail) {
        *tail = newNode;
        newNode->next = newNode;
        return;
    }

    struct SimpleNode* current = (*tail)->next;
    struct SimpleNode* prev = *tail;

    // Encontre a posição correta para inserção
    do {
        if (current->data >= data) break;
        prev = current;
        current = current->next;
    } while (current != (*tail)->next);

    newNode->next = current;
    prev->next = newNode;

    // Atualize o tail se necessário
    if (current == (*tail)->next && data < current->data) {
        *tail = newNode;
    } else if (current == (*tail)->next && data > (*tail)->data) {
        *tail = newNode;
    }
}

// Função para procurar um nó na lista circular
struct SimpleNode* searchSimpleNodeCircular(struct SimpleNode* tail, int key) {
    if (!tail) return NULL;

    struct SimpleNode* current = tail->next;
    do {
        if (current->data == key) {
            return current;
        }
        current = current->next;
    } while (current != tail->next);

    return NULL;
}

// Função para deletar o primeiro nó da lista circular
void deleteFirstNodeCircular(struct SimpleNode** tail) {

    // Se a lista estiver vazia, não faça nada
    if (!*tail) return;

    struct SimpleNode* head = (*tail)->next;

    // Se for o único nó na lista
    if (head == *tail) {
        free(head);
        *tail = NULL;
        return;
    }

    // Caso geral: ajuste o ponteiro next do tail e remova o head
    (*tail)->next = head->next;
    free(head);
}

// Função para imprimir a lista circular
void printSimpleNodeCircularList(struct SimpleNode* tail) {
    if (!tail) return;

    struct SimpleNode* current = tail->next;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != tail->next);
    printf("\n");
}

void freeSimpleNodeCircularList(struct SimpleNode** tail) {
    if (!*tail) return;

    struct SimpleNode* current = (*tail)->next;
    struct SimpleNode* nextNode;

    // Desaloque todos os nós, exceto o último
    while (current != *tail) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    // Desaloque o último nó
    free(*tail);
    *tail = NULL;
}

// fim>

// <Busca, inserção e remoção em lista duplamente encadeada com nó cabeça (sem ordenação)
struct DoublyNode {
    int data;
    struct DoublyNode *prev;
    struct DoublyNode *next;
};
// Função para criar um novo nó
struct DoublyNode* createDoublyNode(int data) {
    struct DoublyNode *newNode = (struct DoublyNode*)malloc(sizeof(struct DoublyNode));
    if (newNode == NULL) {
        printf("Erro: Falha ao alocar memória\n");
        exit(1);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}
// Função para inserir um nó no início da lista
void insertDoublyNode(struct DoublyNode **head, int data) {
    struct DoublyNode *newNode = createDoublyNode(data);
    newNode->next = *head;
    if (*head != NULL)
        (*head)->prev = newNode;
    *head = newNode;
}
// Função para remover um nó da lista
void deleteDoublyNode(struct DoublyNode **head, struct DoublyNode *delNode) {
    if (*head == NULL || delNode == NULL)
        return;
    if (*head == delNode)
        *head = delNode->next;

    if (delNode->next != NULL)
        delNode->next->prev = delNode->prev;

    if (delNode->prev != NULL)
        delNode->prev->next = delNode->next;

    free(delNode);
}

// Função para remover o primeiro nó da lista
void deleteFirstDoublyNode(struct DoublyNode **head) {
    // Verificar se a lista está vazia
    if (*head == NULL) 
        return;

    // Guardar o nó a ser deletado
    struct DoublyNode *delNode = *head;

    // Ajustar o ponteiro do cabeçalho para o próximo nó
    *head = delNode->next;

    // Se o próximo nó existir, ajustar seu ponteiro prev para NULL
    if (*head != NULL)
        (*head)->prev = NULL;

    // Liberar o espaço do nó removido
    free(delNode);
}

// Função para buscar um nó na lista
struct DoublyNode* searchDoublyNode(struct DoublyNode *head, int data) {
    struct DoublyNode *temp = head;
    while (temp != NULL) {
        if (temp->data == data)
            return temp;
        temp = temp->next;
    }
    return NULL;
}
// Função para liberar a lista
void freeDoublyNodeList(struct DoublyNode **head) {
    struct DoublyNode *current = *head;
    struct DoublyNode *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL; // Atualizando o ponteiro da cabeça para indicar que a lista está vazia
}
// Função para exibir a lista
void displayDoublyNodeList(struct DoublyNode *head) {
    struct DoublyNode *temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

// fim>

// <Inserção e remoção em fila (FIFO)
struct Queue {
    struct SimpleNode *front, *rear;
};

// Função para criar um novo nó
struct SimpleNode* newSimpleNode(int data) {
    struct SimpleNode* temp = (struct SimpleNode*)malloc(sizeof(struct SimpleNode));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

// Função para criar uma nova fila vazia
struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// Função para adicionar um elemento na fila
void enqueue(struct Queue* q, int data) {
    struct SimpleNode* temp = newSimpleNode(data);
    
    // Se a fila estiver vazia, o novo nó é o front e o rear
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    
    // Adiciona o novo nó no final da fila e atualiza o rear
    q->rear->next = temp;
    q->rear = temp;
}

// Função para remover um elemento da fila
int dequeue(struct Queue* q) {
    // Se a fila estiver vazia, retorna -1
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }
    
    // Armazena o nó anterior do front e move o front para o próximo nó
    struct SimpleNode* temp = q->front;

    int data = temp->data;
    q->front = q->front->next;
    
    // Se o front se tornar NULL, então muda o rear para NULL também
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    // Libera o nó removido
    free(temp);
    return data;
}

// Função para imprimir a fila
void printQueue(struct Queue* q) {
    struct SimpleNode* temp = q->front;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Função para desalocar a fila
void freeQueue(struct Queue* q) {
    struct SimpleNode* current = q->front;
    struct SimpleNode* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    free(q);
}

// fim>

// <Inserção e remoção em pilha (LIFO)
struct Stack {
    struct SimpleNode* top;
};

// Função para criar um novo nó
struct SimpleNode* newSimpleNodeStack(int data) {
    struct SimpleNode* temp = (struct SimpleNode*)malloc(sizeof(struct SimpleNode));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

// Função para criar uma nova pilha vazia
struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = NULL;
    return stack;
}

// Função para adicionar um elemento na pilha
void push(struct Stack* stack, int data) {
    struct SimpleNode* temp = newSimpleNodeStack(data);
    temp->next = stack->top;
    stack->top = temp;
}

// Função para remover um elemento da pilha
int pop(struct Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }

    struct SimpleNode* temp = stack->top;
    int data = temp->data;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

// Função para imprimir a pilha
void printStack(struct Stack* stack) {
    struct SimpleNode* temp = stack->top;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Função para desalocar a pilha
void freeStack(struct Stack* stack) {
    struct SimpleNode* current = stack->top;
    struct SimpleNode* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    free(stack);
}

// fim>

// <gfx
// Interface gráfica usando a biblioteca gfx para visualização da estrutura de dados manipulada: 
//a janela gráfica deve ser criada no início do programa e fechada no encerramento do mesmo. 
//A visualização deve ser atualizada após cada operação realizada na estrutura de dados.


// Função para desenhar uma lista simplesmente encadeada usando gfx
void drawListSimpleList(struct SimpleNode* list, int startX, int startY) {
     
    if(list == NULL) {
       return;
    }
    
    
    int endX = startX + NODE_WIDTH;
    int endY = startY + NODE_HEIGHT;
    
    // desenha um retangulo preenchido para o nó
    gfx_filled_rectangle(startX, startY, endX, endY);
    
    // define a cor para o texto
    gfx_set_color(0,0,0); // Texto preto
    
    char dataStr[10];
    sprintf(dataStr, "%d", list->data);
    int textWidth, textHeight;
    gfx_get_text_size(dataStr, &textWidth, &textHeight);
    gfx_text(startX + (NODE_WIDTH - textWidth) / 2, startY + (NODE_HEIGHT - textHeight) / 2, dataStr);
    
    gfx_set_color(255, 165, 0); // laranja
    
    if(list->next != NULL) {
       int nextStartX = startX + NODE_WIDTH + NODE_SPACING;
       int nextStartY = startY;
       
       gfx_line(endX, startY + NODE_HEIGHT / 2, nextStartX, startY + NODE_HEIGHT / 2);
       
       drawListSimpleList(list->next, nextStartX, nextStartY);
    }
}

// Função para desenhar uma lista simplesmente encadeada circular usando gfx
void drawListSimpleListCircular(struct SimpleNode* list, struct SimpleNode* head, int startX, int startY) {
     
    if(list == NULL) {
       return;
    }
    
    
    int endX = startX + NODE_WIDTH;
    int endY = startY + NODE_HEIGHT;
    
    // desenha um retangulo preenchido para o nó
    gfx_filled_rectangle(startX, startY, endX, endY);
    
    // define a cor para o texto
    gfx_set_color(0,0,0); // Texto preto
    
    char dataStr[10];
    sprintf(dataStr, "%d", list->data);
    int textWidth, textHeight;
    gfx_get_text_size(dataStr, &textWidth, &textHeight);
    gfx_text(startX + (NODE_WIDTH - textWidth) / 2, startY + (NODE_HEIGHT - textHeight) / 2, dataStr);
    
    gfx_set_color(255, 165, 0); // laranja
    
    if(list->next != head) {
       int nextStartX = startX + NODE_WIDTH + NODE_SPACING;
       int nextStartY = startY;
       
       gfx_line(endX, startY + NODE_HEIGHT / 2, nextStartX, startY + NODE_HEIGHT / 2);
       
       drawListSimpleListCircular(list->next, head, nextStartX, nextStartY);
    }
}

// Função para desenhar uma lista duplamente encadeada usando gfx
void drawListDoubly(struct DoublyNode* list, int startX, int startY) {

    if(list == NULL) {
       return;
    }
    
    int endX = startX + NODE_WIDTH;
    int endY = startY + NODE_HEIGHT;
    
    // desenha um retangulo preenchido para o nó
    gfx_filled_rectangle(startX, startY, endX, endY);
    
    // define a cor para o texto
    gfx_set_color(0, 0, 0); // Texto preto
    
    char dataStr[10];
    sprintf(dataStr, "%d", list->data);
    int textWidth, textHeight;
    gfx_get_text_size(dataStr, &textWidth, &textHeight);
    gfx_text(startX + (NODE_WIDTH - textWidth) / 2, startY + (NODE_HEIGHT - textHeight) / 2, dataStr);
    
    // Desenha a linha para o próximo nó se houver
    gfx_set_color(255, 165, 0); // laranja
    
    if(list->next != NULL) {
       int nextStartX = startX + NODE_WIDTH + NODE_SPACING;
       int nextStartY = startY;
       
       // Desenha a linha para o próximo nó
       gfx_line(endX, startY + NODE_HEIGHT / 2, nextStartX, startY + NODE_HEIGHT / 2);
       
       drawListDoubly(list->next, nextStartX, nextStartY);
    }
    
    // Desenha a linha para o nó anterior se houver
    if(list->prev != NULL) {
        int prevEndX = startX - NODE_SPACING;
        int prevEndY = startY + NODE_HEIGHT / 2 - NODE_HEIGHT / 4; // ajusta a altura da linha para o nó anterior

        // Desenha a linha para o nó anterior
        gfx_line(startX, startY + NODE_HEIGHT / 2 - NODE_HEIGHT / 4, prevEndX, prevEndY);
    }
}

// gfx>

// Função do menu principal e submenus
int Menu(int option) {
   
   do {
     
     system("clear");
     printf("\n\t1 - lista simplesmente encadeada sem nó cabeça ordenada\n\t2 - lista circular simplesmente encadeada sem nó cabeça ordenada\n\t3 - lista duplamente encadeada com nó cabeça (sem ordenação)\n\t4 - fila (FIFO)\n\t5 - pilha (LIFO)\n\t6 - Sair\n");
     scanf("%d", &option);
   }while(option != 1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 6);
   
   // lista simplesmente encadeada sem nó cabeça ordenada
   if(option == 1) {
    int OptionSimpleNode = 0;
    struct SimpleNode* simpleList = NULL;
    
    do {
      //system("clear");
      printf("\n\tlista simplesmente encadeada sem nó cabeça ordenada\n");
      do{
        printf("\n\t1 - Inserir\n\t2 - Remover\n\t3 - Busca\n\t4 - Visualização textual\n\t5 - Sair\n");
        scanf("%d", &OptionSimpleNode);
      }while(OptionSimpleNode != 1 && OptionSimpleNode != 2 && OptionSimpleNode != 3 && OptionSimpleNode != 4 && OptionSimpleNode != 5);
      
      int value = 0;
      // inserir
      if(OptionSimpleNode == 1) {
        printf("\n\tDigite o valor a ser inserido: ");
        scanf("%d", &value);
        insertSimpleNode(&simpleList, value);
        
        gfx_clear();
        gfx_set_color(255, 165, 0); // laranja
        drawListSimpleList(simpleList, 50, 50);
        gfx_paint();
        
      }
      
      // deletar
      if(OptionSimpleNode == 2) { 
        removeFirstNodeSimpleNode(&simpleList);
        
        gfx_clear();
        gfx_set_color(255, 165, 0); // laranja
        drawListSimpleList(simpleList, 50, 50);
        gfx_paint();
      }
      
      // busca
      if(OptionSimpleNode == 3) {
            printf("\n\tDigite o valor a ser buscado: ");
            scanf("%d", &value);
            struct SimpleNode* found = searchSimpleNode(simpleList, value);
            if (found) {
              printf("\n\tElemento %d encontrado: %d\n", value, found->data);
            } else {
              printf("\n\tElemento %d não encontrado\n", value);
            }
      }

      // Visualização textual
      if(OptionSimpleNode == 4) {
        printSimpleList(simpleList);
      }

    }while(OptionSimpleNode != 5);

      // desalocar Lista
      if(simpleList != NULL) {
        freeSimpleList(&simpleList);
      }      
   }
   
   // lista circular simplesmente encadeada sem nó cabeça ordenada
   if(option == 2) {
    int OptionSimpleNodeCircular = 0;
    struct SimpleNode* circularList = NULL;

    do {
      printf("\n\tlista circular simplesmente encadeada sem nó cabeça ordenada\n");
      do{
        printf("\n\t1 - Inserir\n\t2 - Remover\n\t3 - Busca\n\t4 - Visualização textual\n\t5 - Sair\n");
        scanf("%d", &OptionSimpleNodeCircular);
      }while(OptionSimpleNodeCircular != 1 && OptionSimpleNodeCircular != 2 && OptionSimpleNodeCircular != 3 && OptionSimpleNodeCircular != 4 && OptionSimpleNodeCircular != 5);
       
       int value = 0;
       
       // inserir
       if(OptionSimpleNodeCircular == 1) {
            printf("\n\tDigite o valor a ser inserido: ");
            scanf("%d", &value);
            insertSimpleNodeCircular(&circularList, value);
            
            gfx_clear();
            gfx_set_color(255, 165, 0); // laranja
            drawListSimpleListCircular(circularList, circularList, 50, 50);
            gfx_paint();
       }
       
       // deletar
       if(OptionSimpleNodeCircular == 2) {
        deleteFirstNodeCircular(&circularList);
        
        gfx_clear();
        gfx_set_color(255, 165, 0); // laranja
        drawListSimpleListCircular(circularList, circularList, 50, 50);
        gfx_paint();
        
       }
       
       // busca
       if(OptionSimpleNodeCircular == 3) {
            printf("\n\tDigite o valor a ser buscado: ");
            scanf("%d", &value); 
            struct SimpleNode* found = searchSimpleNodeCircular(circularList, value);
            if (found) {
              printf("\n\tEncontrado: %d\n", found->data);
            } else {
              printf("\n\tNão encontrado\n");
            }
       }
        
       // Visualização textual
       if(OptionSimpleNodeCircular == 4) {
         printSimpleNodeCircularList(circularList);
       }

    }while(OptionSimpleNodeCircular != 5);
      
      // desaloca
      if(circularList != NULL) {
        freeSimpleNodeCircularList(&circularList);
      }
   }
   
   // lista duplamente encadeada com nó cabeça (sem ordenação)
   if(option == 3) {
    int OptionDoublyList = 0;
    struct DoublyNode *doublyList = NULL;

    do {
      printf("\n\tlista duplamente encadeada com nó cabeça (sem ordenação)\n");
      do{
        printf("\n\t1 - Inserir\n\t2 - Remover\n\t3 - Busca\n\t4 - Visualização textual\n\t5 - Sair\n");
        scanf("%d", &OptionDoublyList);
      }while(OptionDoublyList != 1 && OptionDoublyList != 2 && OptionDoublyList != 3 && OptionDoublyList != 4 && OptionDoublyList != 5);
      
      int value = 0;
      
      // inserir
      if(OptionDoublyList == 1) {
            printf("\n\tDigite o valor a ser inserido: ");
            scanf("%d", &value);
            insertDoublyNode(&doublyList, value);
            
            gfx_clear();
            gfx_set_color(255, 165, 0); // laranja
            drawListDoubly(doublyList, 50, 50);
            gfx_paint();
      }
      
      // deletar
      if(OptionDoublyList == 2) {
        deleteFirstDoublyNode(&doublyList);
        
        gfx_clear();
        gfx_set_color(255, 165, 0); // laranja
        drawListDoubly(doublyList, 50, 50);
        gfx_paint();
      }
      
      // busca
      if(OptionDoublyList == 3) {
           printf("\n\tDigite o valor a ser buscado: ");
           scanf("%d", &value);
    
           struct DoublyNode *found = searchDoublyNode(doublyList, value);
           if (found != NULL) {
             printf("\n\tnó com valor %d encontrado\n", found->data);
           }else {
             printf("\n\tNó não encontrado\n");
           }
      }
      
      // Visualização textual
      if(OptionDoublyList == 4) {
         displayDoublyNodeList(doublyList);
      }

    }while(OptionDoublyList != 5);

    if(doublyList != NULL) {
        freeDoublyNodeList(&doublyList);
    }
   }
   
   // fila (FIFO)
   if(option == 4) {
    int OptionQueue = 0;
    struct Queue* q = createQueue();

    do {
      printf("\n\tFila(FIFO)\n");
      do{
        printf("\n\t1 - Inserir\n\t2 - Remover\n\t3 - Visualização textual\n\t4 - Sair\n");
        scanf("%d", &OptionQueue);
      }while(OptionQueue != 1 && OptionQueue != 2 && OptionQueue != 3 && OptionQueue != 4);
    
      int value = 0;
      
      // inserir
      if(OptionQueue == 1) {
       printf("\nDigite o valor a ser inserido: ");
       scanf("%d", &value);
       enqueue(q, value);
       
       gfx_clear();
       gfx_set_color(255, 165, 0); // laranja
       drawListSimpleList(q->front, 50, 50);
       gfx_paint();
      } 
      
      // deletar
      if(OptionQueue == 2) {
        dequeue(q);
        
        gfx_clear();
        gfx_set_color(255, 165, 0); // laranja
        drawListSimpleList(q->front, 50, 50);
        gfx_paint();
      }
      
      // Visualização textual
      if(OptionQueue == 3) {
        printQueue(q);
      }

    }while(OptionQueue != 4);

    // desaloca
    freeQueue(q);
   }
   
   // pilha (LIFO)
   if(option == 5) {
    int OptionStack = 0;
    struct Stack* stack = createStack();
    
    do {
      printf("\n\tpilha (LIFO)\n");
      do{
        printf("\n\t1 - Inserir\n\t2 - Remover\n\t3 - Visualização textual\n\t4 - Sair\n");
        scanf("%d", &OptionStack);
      }while(OptionStack != 1 && OptionStack != 2 && OptionStack != 3 && OptionStack != 4);
    
      int value = 0;
      
      // inserir
      if(OptionStack == 1) {
           printf("\n\tDigite o valor a ser inserido: ");
           scanf("%d", &value);
           push(stack, value);
            
           gfx_clear();
           gfx_set_color(255, 165, 0); // laranja
           drawListSimpleList(stack->top, 50, 50);
           gfx_paint();
      }
      
      // deletar
      if(OptionStack == 2) {
        pop(stack);
        
        gfx_clear();
        gfx_set_color(255, 165, 0); // laranja
        drawListSimpleList(stack->top, 50, 50);
        gfx_paint();
      }
      
      // Visualização textual
      if(OptionStack == 3) {
        printStack(stack);
      }

    }while(OptionStack != 4);

    // desaloca
    freeStack(stack);
   }

   return option;
}

int main() {

    gfx_init(WIDTH, HEIGHT, "Visualização");

    int option = 0;
    
    do {
       gfx_clear();
       gfx_paint();
       option = Menu(option);
    }while(option != 6);

    gfx_quit();

    return 0;
}
