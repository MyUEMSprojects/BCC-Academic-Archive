#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Nome: Felipe Echeverria Vilhalva RGM:45611
// Sistemas Operacionais T6
// Prof: Fabricio Sergio de paula

struct Node{
  int key;
  struct Node *prev, *next;
};

struct Node* createNode(int key);
void insertNode(struct Node **head, struct Node *node);
void removeTail(struct Node **head);

// procura um target no array
int array_search(int *array, size_t array_size, int target);
// procura na lista por uma chgave
struct Node* search(struct Node* ptr, int key);
// inicializa o array
int* initArray(size_t size);

// acha o indice da pag cujo prox uso é mais distante
int getOp(int* page_access, int* pages, size_t access_size, size_t page_size, size_t access_index, size_t frame_size);

void FIFO(FILE* fp, int *page_access, size_t access_size, size_t page_size, size_t frame_size);
void LRU(FILE* fp, int *page_access, size_t access_size, size_t page_size, size_t frame_size);
void OPT(FILE* fp, int *page_access, size_t access_size, size_t page_size, size_t frame_size);

int main(int argc, char **argv)
{

  if(argc != 4) {
    printf("Usage: %s <page_size> <memory_size> <filename>\n", argv[0]);
    return -1;
  }

  FILE *fp = fopen(argv[3], "r");
  if(!fp)
  {
    printf("File %s not found!\n", argv[3]);
    return -1;
  }

  fseek(fp, 0L, SEEK_END);
  long file_size = ftell(fp)/sizeof(unsigned short);
  fseek(fp, 0L, SEEK_SET);

  int *page_access = malloc(sizeof(int) * file_size);
  size_t access_current_size = 0;
  int read;

  size_t frames = atoi(argv[2])/atoi(argv[1]);
  size_t page_size = atoi(argv[1]);

  while(fscanf(fp, "%d", &read) != EOF) page_access[access_current_size++] = read;

  fclose(fp);

  fp = fopen("erros.out", "w");

  fprintf(fp, "FIFO:\n");
  FIFO(fp, page_access, access_current_size, page_size, frames);
  fprintf(fp, "\nOPT:\n");
  OPT(fp, page_access, access_current_size, page_size, frames);
  fprintf(fp, "\nLRU:\n");
  LRU(fp, page_access, access_current_size, page_size, frames);

  fclose(fp);
  free(page_access);
  return 0;
}

struct Node* createNode(int key)
{
  struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
  if(!new_node) {printf("Alocation error\n"); exit(1);}
  new_node->key = key;
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}

void insertNode(struct Node **head, struct Node *node)
{
  if(!*head){
    *head = node;
    return;
  }

  if(*head == node) return;

  if(node->prev)
  {
    node->prev->next = node->next;
  }

  if(node->next)
  {
    node->next->prev = node->prev;
  }

  node->next = *head;
  node->prev = NULL;
  (*head)->prev = node;
  *head = node;
}

void removeTail(struct Node **head)
{
  if(!*head) return;
  struct Node* tail = *head;
  while(tail->next) tail = tail->next;
  if(tail->prev) tail->prev->next = NULL;
  else *head = NULL;
  free(tail);
}

int array_search(int *array, size_t array_size, int target)
{
  for(int i = 0; i < array_size; i++)
  {
    if(array[i] == target) return i;
  }

  return -1;
}

struct Node* search(struct Node* ptr, int key)
{
  if(!ptr) return NULL;
  while(ptr)
  {
    if(ptr->key == key) return ptr;
    ptr = ptr->next;
  }
  return NULL;
}

int* initArray(size_t size)
{
  int *array = (int*) malloc(sizeof(int) * size);
  if(!array)
  {
    perror("Alocation error\n");
    exit(1);
  }

  for(int i = 0; i < size; i++)
  {
    array[i] = -1;
  }

  return array;
}

int getOp(int* page_access, int* pages, size_t access_size, size_t page_size, size_t access_index, size_t frame_size)
{
    for (int i = 0; i < frame_size; i++) {
        if (pages[i] == -1) {
            return i;
        }
    }

    int page_to_replace = 0;
    int max_distance = -1; // a distancia maxima até o prox uso

    for (int i = 0; i < frame_size; i++) {
        int current_page_in_frame = pages[i];
        int distance = 0; // distancia até o prox uso
        int found = 0;   // flag de uso para verificar o uso futuro foi encontrado

        // procurar o prox uso da pag
        for (int j = access_index + 1; j < access_size; j++) {
            int future_page = page_access[j] / page_size;

            if (future_page == current_page_in_frame) {
                distance = j;
                found = 1;
                break; // achou prox uso, break para nao verificar mais a pag
            }
        }

        // pag nunca sera usada no futuro
        if (found == 0) {
            return i; // melhor pag para substituir
        }

        // achou uso no futuro, ai verificamos se é a mais distante
        if (distance > max_distance) {
            max_distance = distance;
            page_to_replace = i;
        }
    }
    return page_to_replace;
}

void FIFO(FILE* fp, int *page_access, size_t access_size, size_t page_size, size_t frame_size)
{
  size_t faults = 0;
  int *pages = initArray(frame_size);
  for(int i = 0; i < access_size; i++)
  {
    int page = page_access[i] / page_size;
    if(array_search(pages, frame_size, page) == -1)
    {
      pages[faults % frame_size] = page;
      fprintf(fp, "erro de pagina endereço :%d pagina: %d\n", page_access[i], page);
      faults++;
    }
  }

  free(pages);
  printf("FIFO: %ld erros (%.2f%%)\n", faults, (faults * 100.0) / access_size);
}
void LRU(FILE* fp, int *page_access, size_t access_size, size_t page_size, size_t frame_size)
{
  struct Node* pages = NULL;
  size_t faults = 0;

  for(int i = 0; i < access_size; i++)
  {
    int page = page_access[i] / page_size;
    struct Node *hit = search(pages, page);
    if(!hit)
    {
      if(faults >= frame_size) removeTail(&pages);
      insertNode(&pages, createNode(page));
      fprintf(fp, "erro de pagina endereço :%d pagina: %d\n", page_access[i], page);
      faults++;
    }
    else insertNode(&pages, hit);
  }
  while(pages) removeTail(&pages);
  printf("LRU: %ld erros (%.2f%%)\n", faults, (faults * 100.0) / access_size);

}
void OPT(FILE* fp, int *page_access, size_t access_size, size_t page_size, size_t frame_size)
{
  int* pages = initArray(frame_size);
  size_t faults = 0;
  for(int i = 0; i < access_size; i++)
  {
    int page = page_access[i] / page_size;
    if(array_search(pages, frame_size, page) == -1)
    {
      pages[getOp(page_access, pages, access_size, page_size, i, frame_size)] = page;
      fprintf(fp, "erro de pagina endereço :%d pagina: %d\n", page_access[i], page);
      faults++;
    }
  }

  free(pages);
  printf("OPT: %ld erros (%.2f%%)\n", faults, (faults * 100.0) / access_size);
}