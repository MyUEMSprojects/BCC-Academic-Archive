#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

/*
Nome: Felipe Echeverria Vilhalva  RGN: 45611
Trabalho 5 - Sistemas Operacionais
Professor: Fabricio Sergio de Paula
*/

// flags de sincronizacao do pipeline(controle basicamente)
#define CREATED 1
#define REMOVED_PAIRS 2
#define REMOVED_NOT_PRIMES 3

struct Node
{
    int value;
    pthread_mutex_t mutex;
    int status;
    struct Node *next;
};

// insere o nó na lista
void insertNode(int value, struct Node **ptr);
// deleta um nó e seu mutex
void deleteNode(struct Node *node);
// verifica se o numero do argumento é primo
int is_prime(int number);
// remove os pares maiores q 2
void *remove_pairs_runner(void *args);
// remove os não-primos
void *remove_not_primes_runner(void *args);
// printa os primos restantes
void *print_runner(void *args);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("%s <file_name>", argv[0]);
        exit(1);
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        perror("file not found!");
        exit(1);
    }

    struct Node *list = NULL;

    pthread_t thread_remove_pairs_runner, thread_remove_not_primes_runner, thread_print_runner;

    pthread_create(&thread_remove_pairs_runner, NULL, remove_pairs_runner, &list);
    pthread_create(&thread_remove_not_primes_runner, NULL, remove_not_primes_runner, &list);
    pthread_create(&thread_print_runner, NULL, print_runner, &list);

    int value; // valor a ser lido
    struct Node *tail = NULL; // evitar percorrer a lista inteira em loop
    while (fscanf(fp, "%d", &value) != EOF)
    {
        if(!tail)
        {
            insertNode(value, &list);
            tail = list;
        }
        else
        {
            insertNode(value, &tail);
            tail = tail->next;
        }
    }

    insertNode(-1, &list); // acabou

    pthread_join(thread_remove_pairs_runner, NULL);
    pthread_join(thread_remove_not_primes_runner, NULL);
    pthread_join(thread_print_runner, NULL);

    fclose(fp);
    return 0;
}

void insertNode(int value, struct Node **ptr)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    if (!new_node)
    {
        printf("Error allocating memory of Node with value %d\n", value);
        exit(1);
    }

    new_node->next = NULL;
    new_node->value = value;

    if (pthread_mutex_init(&new_node->mutex, NULL) == -1)
    {
        printf("\n Error creating semaphore of Node %d\n", value);
        exit(1);
    }

    pthread_mutex_lock(&new_node->mutex);
    new_node->status = CREATED;

    struct Node *current = *ptr;

    if (!current)
    {
        *ptr = new_node;
    }
    else
    {
        while (current->next) current = current->next;
        current->next = new_node;
        pthread_mutex_unlock(&current->mutex);
    }

    if (value == -1) pthread_mutex_unlock(&new_node->mutex);
}

void deleteNode(struct Node *node)
{
    pthread_mutex_unlock(&node->mutex);
    pthread_mutex_destroy(&node->mutex);
    free(node);
}

int is_prime(int number)
{
    if (number == 2) {
        return 1;
    }

    if (number % 2 == 0 || number <= 1) {
        return 0;
    }

    for (int i = 3; i <= number / i; i += 2) {
        if (number % i == 0) {
            return 0;
        }
    }

    return 1;
}

void *remove_pairs_runner(void *args)
{
    struct Node **head = (struct Node **)args;

    while (!(*head));

    struct Node *current = *head;
    struct Node *prev = NULL;

    while (true)
    {
        pthread_mutex_lock(&current->mutex);

        if (current->value == -1)
        {
            current->status = REMOVED_PAIRS;
            pthread_mutex_unlock(&current->mutex);
            if (prev) pthread_mutex_unlock(&prev->mutex);
            break;
        }

        if (current->value % 2 == 0 && current->value > 2) // valor é par maior q 2
        {
            // próximo mutex
            pthread_mutex_lock(&current->next->mutex);

            if (!prev) *head = current->next;
            else prev->next = current->next;

            struct Node *nodeDeleted = current;
            current = current->next;
            deleteNode(nodeDeleted);
            pthread_mutex_unlock(&current->mutex);
            continue;
        }

        if (prev) pthread_mutex_unlock(&prev->mutex);

        pthread_mutex_lock(&current->next->mutex);
        prev = current;
        current->status = REMOVED_PAIRS;
        current = current->next;
        pthread_mutex_unlock(&current->mutex);
    }
    pthread_exit(NULL);
}

void *remove_not_primes_runner(void *args)
{
    struct Node **head = (struct Node **)args;

    while (!((*head) && (*head)->status == REMOVED_PAIRS));

    struct Node *current = *head;
    struct Node *prev = NULL;

    while (true)
    {
        pthread_mutex_lock(&current->mutex);

        if (current->value == -1)
        {
            current->status = REMOVED_NOT_PRIMES;
            pthread_mutex_unlock(&current->mutex);
            if (prev) pthread_mutex_unlock(&prev->mutex);
            break;
        }

        if (!is_prime(current->value))
        {
            pthread_mutex_lock(&current->next->mutex); // a exclusão pode causar erros, ai precisa do mutex do próximo

            if (!prev) *head = current->next;
            else prev->next = current->next;

            struct Node *nodeDeleted = current;
            current = current->next;
            deleteNode(nodeDeleted);
            pthread_mutex_unlock(&current->mutex);

            continue;
        }

        if (prev) pthread_mutex_unlock(&prev->mutex);

        pthread_mutex_lock(&current->next->mutex);
        prev = current;
        current->status = REMOVED_NOT_PRIMES;
        current = current->next;
        pthread_mutex_unlock(&current->mutex);
    }

    pthread_exit(NULL);
}

void *print_runner(void *args)
{
    struct Node **head = (struct Node **)args;
    while (!((*head) && (*head)->status == REMOVED_NOT_PRIMES));
    struct Node *current = *head;

    while (true)
    {
        pthread_mutex_lock(&current->mutex);

        if (current->value == -1)
        {
            deleteNode(current);
            break;
        }

        printf("%d ", current->value);

        pthread_mutex_lock(&current->next->mutex);
        struct Node *nodeDeleted = current;
        current = current->next;
        deleteNode(nodeDeleted);
        pthread_mutex_unlock(&current->mutex);
    }

    pthread_exit(NULL);
}