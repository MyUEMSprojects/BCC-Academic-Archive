#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>

/*
Nome: Felipe Echeverria Vilhalva RGM: 45611
Professor: Fabricio Sergio de Paula - Sistemas Operacionais

// sem memoria compartilhada
*/

// struct args para passar como parametro para o runner
struct Args {
    int start;
    int end;
    int thread_id;
};

int *array = NULL;  // array com os numeros lidos do arquivo
int *sum = NULL;

int readfile(char *file_name, int **array);
void *runner(void *param);

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "uso: %s <nome do arquivo> <numero de threads>\n", argv[0]);
        return -1;
    }

    int qtd_threads = atoi(argv[2]);
    if(qtd_threads <= 0) {
        fprintf(stderr, "O numero de threads deve ser > 0\n");
        return -1;
    }

    char *file_name = strdup(argv[1]);
    if(!file_name) {
        perror("Erro em strdup()");
        return -1;
    }

    // ler arquivo e definir numeros
    int qtd_numbers = readfile(file_name, &array);
    if(qtd_numbers <= 0) {
        fprintf(stderr, "Erro ao ler arquivos ou arquivo vazio\n");
        free(file_name);
        return -1;
    }

    sum = malloc(sizeof(int) * qtd_threads);
    if (!sum) {
        perror("Erro ao alocar memoria para soma");
        free(array);
        free(file_name);
        return -1;
    }

    pthread_t threads[qtd_threads];
    struct Args thread_args[qtd_threads]; // Array de argumentos para cada thread

    // Criar threads
    for(int i = 0; i < qtd_threads; i++) {

        // calcula o inicio e o fim da regiao de sum(do array) do processo
        thread_args[i].start = i * (qtd_numbers / qtd_threads);
        thread_args[i].end = (i == qtd_threads - 1) ? qtd_numbers - 1 :
                          (i + 1) * (qtd_numbers / qtd_threads) - 1;
        thread_args[i].thread_id = i; // atribuir o id da thread como o index

	    // cria a thread e faz a verificação de erros
        if(pthread_create(&threads[i], NULL, runner, &thread_args[i]) != 0) {
            perror("Erro ao criar thread");
            return -1;
        }
    }

    // esperando as threads encerrarem
    for(int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calcular soma total
    int total_sum = 0;
    for(int i = 0; i < qtd_threads; i++) {
        total_sum += sum[i];
    }

    printf("Soma: %d\n", total_sum);

    free(sum);
    free(array);
    free(file_name);

    return 0;
}

int readfile(char *file_name, int **array) {
    FILE *file = fopen(file_name, "rb");
    if(!file) {
        perror("Erro ao abrir arquivo!");
        return -1;
    }

    // pega o tamanho do arquivo
    if(fseek(file, 0, SEEK_END) != 0) {
        perror("Erro em fseek()");
        fclose(file);
        return -1;
    }

    long file_size = ftell(file);
    if(file_size == -1) {
        perror("Erro em ftell()");
        fclose(file);
        return -1;
    }

    rewind(file);

    // calcula o numero de inteiros
    long qtd_numbers = file_size / sizeof(int);
    if(qtd_numbers <= 0) {
        fprintf(stderr, "Arquivo vazio!\n");
        fclose(file);
        return -1;
    }

    // aloca a memoria do array
    *array = malloc(file_size);
    if(!*array) {
        perror("Erro em alocar o array");
        fclose(file);
        return -1;
    }

    // le o conteudo do arquivo
    size_t items_read = fread(*array, sizeof(int), qtd_numbers, file);
    if(items_read != (size_t)qtd_numbers) {
        perror("erro em ler o conteudo do arquivo.");
        free(*array);
        *array = NULL;
        fclose(file);
        return -1;
    }

    fclose(file);
    return qtd_numbers;
}

void *runner(void *param) {
    struct Args *args = (struct Args*)param;
    int partial_sum = 0;

    for(int j = args->start; j <= args->end; j++) {
        partial_sum += array[j];
    }

    sum[args->thread_id] = partial_sum;
    pthread_exit(0);
}