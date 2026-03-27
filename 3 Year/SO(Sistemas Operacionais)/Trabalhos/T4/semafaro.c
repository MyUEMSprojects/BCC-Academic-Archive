#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

struct Node {
    int key;
    struct Node* next;
};

struct List {
    struct Node *head;
    struct Node *tail;
    int size;
};

struct List *L1, *L2, *L3;

const char* SEM_MUTEX_L1 = "/mutex_l1";
const char* SEM_ITENS_L1 = "/itens_l1";
const char* SEM_MUTEX_L2 = "/mutex_l2";
const char* SEM_ITENS_L2 = "/itens_l2";
const char* SEM_MUTEX_L3 = "/mutex_l3";
const char* SEM_ITENS_L3 = "/itens_l3";

sem_t *mutex_l1, *itens_l1;
sem_t *mutex_l2, *itens_l2;
sem_t *mutex_l3, *itens_l3;

const int SPY = -1;

struct Node* create_node(int key);
struct List* create_list();
void insert(struct List *list, int key);
int remove_head(struct List *list);
void free_list(struct List *list);

void* analisa_pares(void*arg);
void* analisa_nao_primos(void*arg);
void* imprime_primos(void*arg);

bool is_prime(int n);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }
    const char* input_filename = argv[1];

    char *output_filename = malloc(strlen(input_filename) + 5);
    if (!output_filename) {
        perror("Falha ao alocar memória para o nome do arquivo de saída");
        return 1;
    }
    sprintf(output_filename, "%s.out", input_filename);

    L1 = create_list();
    L2 = create_list();
    L3 = create_list();

    if (!L1 || !L2 || !L3) {
        fprintf(stderr, "Erro ao alocar memória para as listas.\n");
        free(output_filename);
        return 1;
    }

    sem_unlink(SEM_MUTEX_L1);
    sem_unlink(SEM_ITENS_L1);
    sem_unlink(SEM_MUTEX_L2);
    sem_unlink(SEM_ITENS_L2);
    sem_unlink(SEM_MUTEX_L3);
    sem_unlink(SEM_ITENS_L3);

    mutex_l1 = sem_open(SEM_MUTEX_L1, O_CREAT, 0644, 1);
    itens_l1 = sem_open(SEM_ITENS_L1, O_CREAT, 0644, 0);
    mutex_l2 = sem_open(SEM_MUTEX_L2, O_CREAT, 0644, 1);
    itens_l2 = sem_open(SEM_ITENS_L2, O_CREAT, 0644, 0);
    mutex_l3 = sem_open(SEM_MUTEX_L3, O_CREAT, 0644, 1);
    itens_l3 = sem_open(SEM_ITENS_L3, O_CREAT, 0644, 0);

    if (mutex_l1 == SEM_FAILED || itens_l1 == SEM_FAILED ||
        mutex_l2 == SEM_FAILED || itens_l2 == SEM_FAILED ||
        mutex_l3 == SEM_FAILED || itens_l3 == SEM_FAILED) {
        perror("sem_open");
        free(output_filename);
        return 1;
    }

    pthread_t threads[3];

    if (pthread_create(&threads[0], NULL, analisa_pares, NULL) != 0) {
        fprintf(stderr, "Erro ao criar a thread que analisa os pares\n");
    }
    if (pthread_create(&threads[1], NULL, analisa_nao_primos, NULL) != 0) {
        fprintf(stderr, "Erro ao criar a thread que analisa os nao primos\n");
    }
    if (pthread_create(&threads[2], NULL, imprime_primos, output_filename) != 0) {
        fprintf(stderr, "Erro ao criar a thread que imprime os numeros em L3\n");
    }

    FILE *file = fopen(input_filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de entrada");
        insert(L1, SPY);
        sem_post(itens_l1);
    } else {
        int num;
        printf("Thread Principal: Lendo números de '%s'...\n", input_filename);
        while (fscanf(file, "%d", &num) == 1) {
            sem_wait(mutex_l1);
            insert(L1,num);
            sem_post(mutex_l1);
            sem_post(itens_l1);
        }
        fclose(file);
    }

    sem_wait(mutex_l1);
    insert(L1, SPY);
    sem_post(mutex_l1);
    sem_post(itens_l1);
    printf("Thread Principal: Leitura finalizada. Sentinela enviado.\n");

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Thread Principal: Todas as threads finalizaram.\n");

    free_list(L1);
    free_list(L2);
    free_list(L3);

    free(output_filename);

    sem_close(mutex_l1);
    sem_close(itens_l1);
    sem_close(mutex_l2);
    sem_close(itens_l2);
    sem_close(mutex_l3);
    sem_close(itens_l3);

    sem_unlink(SEM_MUTEX_L1);
    sem_unlink(SEM_ITENS_L1);
    sem_unlink(SEM_MUTEX_L2);
    sem_unlink(SEM_ITENS_L2);
    sem_unlink(SEM_MUTEX_L3);
    sem_unlink(SEM_ITENS_L3);

    return 0;
}

void* imprime_primos(void* arg) {
    const char* filename = (const char*)arg;

    FILE *outFile = fopen(filename, "w");
    if (outFile == NULL) {
        perror("Erro ao criar o arquivo de saída");
        pthread_exit(NULL);
    }

    printf("Thread Impressora: Aguardando números primos para salvar em '%s'...\n", filename);
    while (1) {
        sem_wait(itens_l3);
        sem_wait(mutex_l3);
        int num = remove_head(L3);
        sem_post(mutex_l3);

        if (num == SPY) {
            break;
        }

        fprintf(outFile, "%d\n", num);
    }

    fclose(outFile);
    printf("Thread Impressora: Finalizada. Arquivo '%s' foi gerado.\n", filename);
    pthread_exit(NULL);
}

void* analisa_pares(void*arg) {
    while (1) {
        sem_wait(itens_l1);
        sem_wait(mutex_l1);
        int num = remove_head(L1);
        sem_post(mutex_l1);

        if (num == SPY) {
            sem_wait(mutex_l2);
            insert(L2, SPY);
            sem_post(mutex_l2);
            sem_post(itens_l2);
            break;
        }

        if (num % 2 != 0 || num == 2) {
            sem_wait(mutex_l2);
            insert(L2, num);
            sem_post(mutex_l2);
            sem_post(itens_l2);
        }
    }
    pthread_exit(NULL);
}

void* analisa_nao_primos(void*arg) {
    while (1) {
        sem_wait(itens_l2);
        sem_wait(mutex_l2);
        int num = remove_head(L2);
        sem_post(mutex_l2);

        if (num == SPY) {
            sem_wait(mutex_l3);
            insert(L3, SPY);
            sem_post(mutex_l3);
            sem_post(itens_l3);
            break;
        }

        if (is_prime(num)) {
            sem_wait(mutex_l3);
            insert(L3, num);
            sem_post(mutex_l3);
            sem_post(itens_l3);
        }
    }
    pthread_exit(NULL);
}

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

struct Node* create_node(int key) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    if (!new_node) return NULL;
    new_node->key = key;
    new_node->next = NULL;
    return new_node;
}

struct List* create_list() {
    struct List *list = (struct List*)malloc(sizeof(struct List));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void insert(struct List *list, int key) {
    if (!list) return;
    struct Node* new_node = create_node(key);
    if (!new_node) return;

    if (list->size == 0) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

int remove_head(struct List *list) {
    if (!list || list->size == 0) return -1;

    struct Node* node_to_remove = list->head;
    int key = node_to_remove->key;

    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
    }

    free(node_to_remove);
    list->size--;
    return key;
}

void free_list(struct List *list) {
    if (!list) return;
    struct Node* current = list->head;
    while (current != NULL) {
        struct Node* node_to_free = current;
        current = current->next;
        free(node_to_free);
    }
    free(list);
}