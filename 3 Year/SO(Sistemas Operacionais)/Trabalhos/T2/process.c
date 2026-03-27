#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
Nome: Felipe Echeverria Vilhalva RGM: 45611
Professor: Fabricio Sergio de Paula - Sistemas Operacionais
*/

int *array = NULL;  // array com os numeros lidos do arquivo
int *sum = NULL;   // memoria compartilhada com as somas parciais

int readfile(char *file_name, int **array);

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Uso: %s <nome_arquivo> <numero_de_processos>\n", argv[0]);
        return -1;
    }

    int qtd_process = atoi(argv[2]);

    if(qtd_process < 0)
    {
        fprintf(stderr, "O numero de processos deve ser >= 0\n");
        return -1;
    }

    char *file_name = strdup(argv[1]);
    if(!file_name)
    {
        perror("Erro em strdup()");
        return -1;
    }

    // ler arquivo e definir numeros
    int qtd_numbers = readfile(file_name, &array);
    if(qtd_numbers <= 0)
    {
        fprintf(stderr, "Erro ao ler arquivos ou arquivo vazio\n");
        free(file_name);
        return -1;
    }

    // se nao tiver filhos(processos)
    if(qtd_process == 0)
    {
        int total_sum = 0;
        for(int i = 0; i < qtd_numbers; i++)
        {
            total_sum += array[i];
        }
        printf("Soma: %d\n", total_sum);
        free(array);
        free(file_name);
        return -1;
    }

    // cria uma memoria compartilhada para a somas parciais
    int shm_fd = shm_open("/sum_shm", O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1)
    {
        perror("Erro em shm_open()");
        free(array);
        free(file_name);
        return -1;
    }

    // define o tamanho da memoria compartilhada
    if(ftruncate(shm_fd, qtd_process * sizeof(int)) == -1)
    {
        perror("Erro em ftruncate()");
        close(shm_fd);
        shm_unlink("/sum_shm");
        free(array);
        free(file_name);
        return -1;
    }

    // mapear a memoria compartilhada
    sum = mmap(NULL, qtd_process * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(sum == MAP_FAILED)
    {
        perror("Erro em mmap()");
        close(shm_fd);
        shm_unlink("/sum_shm");
        free(array);
        free(file_name);
        return -1;
    }

    // inicializa a memoria compartilhada
    memset(sum, 0, qtd_process * sizeof(int));

    // cria um array de pids
    pid_t pid[qtd_process];
    for(int i = 0; i < qtd_process; i++)
    {
        pid[i] = fork(); // cria um processo com seu identificador próprio
        if(pid[i] == 0) // processo filho
        {
            // calcula o inicio e o fim da regiao de sum(do array) do processo
            int start = i * (qtd_numbers / qtd_process);
            int end = (i == qtd_process - 1) ? qtd_numbers - 1 :
                       (i + 1) * (qtd_numbers / qtd_process) - 1;

            int partial_sum = 0;
            for(int j = start; j <= end; j++)
            {
                partial_sum += array[j];
            }
            sum[i] = partial_sum;

            // limpar os recursos usados pelo filho
            munmap(sum, qtd_process * sizeof(int));
            close(shm_fd);
            exit(0);
        }
        else if(pid[i] < 0)
        {
            perror("Erro no fork");

            // limpar os todos os outros filhos em caso de erro em 1
            for(int j = 0; j < i; j++)
            {
                waitpid(pid[j], NULL, 0);
            }

            munmap(sum, qtd_process * sizeof(int));
            close(shm_fd);
            shm_unlink("/sum_shm");
            free(array);
            free(file_name);
            return -1;
        }
    }

    // pai espera os filhos terminarem
    for(int i = 0; i < qtd_process; i++)
    {
        waitpid(pid[i], NULL, 0);
    }


    // calcula a sum total
    int total_sum = 0;
    for(int i = 0; i < qtd_process; i++)
    {
        total_sum += sum[i];
    }
    printf("Soma: %d\n", total_sum);

    // limpar tudo
    munmap(sum, qtd_process * sizeof(int));
    close(shm_fd);
    shm_unlink("/sum_shm");
    free(array);
    free(file_name);

    return 0;
}

int readfile(char *file_name, int **array)
{
    FILE *file = fopen(file_name, "rb");
    if(!file)
    {
        perror("Erro ao abrir arquivo!");
        return -1;
    }

    if(fseek(file, 0, SEEK_END) != 0)
    {
        perror("Erro em fseek()");
        fclose(file);
        return -1;
    }

    // pega o tamanho do arquivo
    long file_size = ftell(file);
    if(file_size == -1)
    {
        perror("Erro() em ftell()");
        fclose(file);
        return -1;
    }

    rewind(file);

    // calcula o numero de inteiros
    long qtd_numbers = file_size / sizeof(int);
    if(qtd_numbers <= 0)
    {
        fprintf(stderr, "Arquivo vazio!\n");
        fclose(file);
        return -1;
    }

    // aloca a memoria do array
    *array = malloc(file_size);
    if(!*array)
    {
        perror("Erro em alocar o array");
        fclose(file);
        return -1;
    }

    // le o conteudo do arquivo
    size_t items_read = fread(*array, sizeof(int), qtd_numbers, file);
    if(items_read != (size_t)qtd_numbers)
    {
        perror("erro em ler o conteudo do arquivo.");
        free(*array);
        *array = NULL;
        fclose(file);
        return -1;
    }

    fclose(file);
    return qtd_numbers;
}