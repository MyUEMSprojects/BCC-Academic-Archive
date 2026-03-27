#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAMANHO_BYTE 256
#define TAMANHO_VERIFICACAO 1024 // Tamanho do buffer para verificação
#define TAMANHO_HEAP 4096

struct No
{
    unsigned char byte;
    int frequencia;
    struct No *esq, *dir;
};

struct Arvore
{
    struct No *raiz;
};

struct HeapMin
{
    struct Arvore *arvores; // array de arvores
    int n;                  // número de arvores
    int capacidade;         // capacidade total do heap
};

struct Serializado
{
    struct Arvore huffman;
    unsigned char* arquivo_compactado;
};

void para_binario(unsigned char valor, char *saida)
{
    for (int i = 0; i < 8; i++)
    {
        saida[7 - i] = (valor & (1 << i)) ? '1' : '0';
    }

    saida[8] = '\0';
}

// Função para contar as frequências dos bytes no arquivo
void calcular_frequencias(FILE *file, int frequencias[256])
{
    unsigned char byte;
    while (fread(&byte, 1, 1, file))
    {
        frequencias[byte]++;
    }
    rewind(file);
}

void exibir_frequencia(int frequencia[256])
{
    char binario[9]; // 8 bits + null-terminator
    for (int i = 0; i < TAMANHO_BYTE; i++)
    {
        if (frequencia[i] > 0)
        {
            para_binario(i, binario); // Converte o valor do byte para binário
            printf("Byte %s (%d): %u vezes\n", binario, i, frequencia[i]);
        }
    }
}

struct No *criarNo(unsigned char byte, int frequencia)
{
    struct No *novo_no = (struct No *)malloc(sizeof(struct No));
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->byte = byte;
    novo_no->frequencia = frequencia;
    return novo_no;
}

struct HeapMin *criar_heap(int capacidade)
{
    struct HeapMin *heap = (struct HeapMin *)malloc(sizeof(struct HeapMin));
    if (!heap)
    {
        perror("Erro ao alocar memória para o heap");
        exit(EXIT_FAILURE);
    }
    heap->capacidade = capacidade;
    heap->n = 0;
    heap->arvores = (struct Arvore *)malloc(capacidade * sizeof(struct Arvore));
    if (!heap->arvores)
    {
        perror("Erro ao alocar memória para as árvores do heap");
        exit(EXIT_FAILURE);
    }
    return heap;
}

// Função auxiliar para trocar duas arvores
void swap_arvore(struct Arvore *a, struct Arvore *b)
{
    struct Arvore temp = *a;
    *a = *b;
    *b = temp;
}

// Função subir: reorganiza o heap após inserção
void subir(struct HeapMin *heap, int i)
{
    // Enquanto não chegar a raiz (i = 0) e o elemento atual for menor que o pai
    while (i != 0 && heap->arvores[(i - 1) / 2].raiz->frequencia > heap->arvores[i].raiz->frequencia)
    {
        // Troca o nó atual com o pai
        swap_arvore(&heap->arvores[i], &heap->arvores[(i - 1) / 2]);
        // Atualiza o indice para o do pai
        i = (i - 1) / 2;
    }
}

// Função descer: reorganiza o heap após remoção
void descer(struct HeapMin *heap, int i)
{
    int menor = i;
    int esquerda = 2 * i + 1; // indice do filho esquerdo
    int direita = 2 * i + 2;  // indice do filho direito

    // Verifica se o filho esquerdo existe e se é menor que o nó atual
    if (esquerda < heap->n && heap->arvores[esquerda].raiz->frequencia < heap->arvores[menor].raiz->frequencia)
    {
        menor = esquerda;
    }

    // Verifica se o filho direito existe e se é menor que o menor até agora
    if (direita < heap->n && heap->arvores[direita].raiz->frequencia < heap->arvores[menor].raiz->frequencia)
    {
        menor = direita;
    }

    // Se o menor não for o próprio nó atual, faz a troca e continua descendo
    if (menor != i)
    {
        swap_arvore(&heap->arvores[i], &heap->arvores[menor]);
        descer(heap, menor);
    }
}

// Função para inserir uma árvore no heap
void inserir_heap(struct HeapMin *heap, struct Arvore arvore)
{
    if (heap->n == heap->capacidade)
    {
        // Se o heap estiver cheio, não pode inserir mais
        printf("Ta cheio!\n");
        return;
    }

    // Insere a nova árvore no final do heap
    heap->arvores[heap->n] = arvore;
    int i = heap->n;
    heap->n++;

    // Reogarniza o heap chamando a função subir
    subir(heap, i);
}

// Função para extrair o elemento com a menor frequencia (a raiz)
struct Arvore extrair_minimo(struct HeapMin *heap)
{
    if (heap->n == 0)
    {
        struct Arvore arvore_vazia = {NULL};
        return arvore_vazia; // Retorna árvore vazia se o heap estiver vazio
        // return;
    }

    // O menor elemento é a raiz
    struct Arvore raiz = heap->arvores[0];

    // Coloca o ultimo elemento na raiz e reduz o tamanho do heap
    heap->arvores[0] = heap->arvores[heap->n - 1];
    heap->n--;

    // Reogarniza o heap chamando a função descer
    descer(heap, 0);

    return raiz;
}

// Função para liberar a memória do heap
void liberar_heap(struct HeapMin *heap)
{
    free(heap->arvores);
    free(heap);
}

void heap_sort(struct HeapMin *heap)
{
    int tamanho_original = heap->n;
    struct Arvore arvores_aux[tamanho_original];

    // Extrai todos os elementos do heap para a arvores_aux
    for (int i = 0; i < tamanho_original; i++)
    {
        arvores_aux[i] = extrair_minimo(heap); // Armazena na ordem crescente
    }

    // Copia os elementos de volta para o heap em ordem crescente
    for (int i = 0; i < tamanho_original; i++)
    {
        heap->arvores[i] = arvores_aux[i];
    }

    // Após o loop, os elementos estão ordenados em ordem crescente
    heap->n = tamanho_original; // Restaurar o valor original de n
}

// Procedimento para construir o heap de frequencias
void construirHeap(struct HeapMin *heap, int frequencia[256], int tamanho_frequencias)
{
    int num_arvores = 0;
    for (int i = 0; i < tamanho_frequencias; i++)
    {
        if (frequencia[i] > 0)
        {
            struct No *novo_no = criarNo(i, frequencia[i]);
            struct Arvore nova_arvore = {novo_no}; // criar uma nova árvore
            inserir_heap(heap, nova_arvore);       // inserir no heap
            num_arvores++;
        }
    }
}

void exibir_heap(struct HeapMin *heap)
{
    printf("Heap:\n");
    for (int i = 0; i < heap->n; i++)
    {
        printf("Árvore %d: Frequência = %d\n", i, heap->arvores[i].raiz->frequencia);
    }
}

// Procedimento para montar a arvore de huffman
struct Arvore montar_arvore(struct HeapMin *heap)
{
    while (heap->n > 1) // Enquanto houver mais de uma árvore no heap
    {
        // printf("Entrou heap->n > 1!\n");
        //  Extrair as duas árvores de menor frequencia
        struct Arvore arvore1 = extrair_minimo(heap);
        struct Arvore arvore2 = extrair_minimo(heap);

        // printf("Extraindo arvore1 com frequencia: %d\n", arvore1.raiz->frequencia);
        // printf("Extraindo arvore2 com frequencia: %d\n", arvore2.raiz->frequencia);

        // Criar um novo nó com a soma das frequencias
        struct No *novo_no = criarNo(-1, arvore1.raiz->frequencia + arvore2.raiz->frequencia);

        // O novo nó tem as duas árvores com filhos
        novo_no->esq = arvore1.raiz;
        novo_no->dir = arvore2.raiz;

        // Criar uma nova árvore com o novo nó como raiz
        struct Arvore nova_arvore;
        nova_arvore.raiz = novo_no;

        // printf("Nova arvore com frequencia: %d\n", nova_arvore.raiz->frequencia);

        // Inserir a nova árvore de volta no heap
        inserir_heap(heap, nova_arvore);
    }

    // Quando restar apenas uma árvore no heap, ela é a árvore de Huffman
    return extrair_minimo(heap);
}

// Procedimento para imprimir a árvore
void imprimir_arvore(struct No *raiz)
{
    if (raiz == NULL)
    {
        return; // Se a raiz for nula, não faz nada
    }
    // Imprime o nó atual com um recuo baseado no nível da árvore
    char binario[9]; // 8 bits + null-terminator
    para_binario(raiz->byte, binario);
    printf("Byte %s (%d): %u vezes(frequencia)\n", binario, raiz->byte, raiz->frequencia);

    // Chama recursivamente para os filhos, aumentando o nível
    imprimir_arvore(raiz->esq);
    imprimir_arvore(raiz->dir);
}

int altura_arvore(struct No *raiz)
{
    int esq, dir;
    if (raiz == NULL)
    {
        return -1;
    }
    else
    {
        esq = altura_arvore(raiz->esq) + 1;
        dir = altura_arvore(raiz->dir) + 1;

        if (esq > dir)
        {
            return esq;
        }
        else
        {
            return dir;
        }
    }
}

// Função auxiliar(codificar) para alocar o dicionario dos novos códigos
char **aloca_dict(int colunas)
{
    char **dict;
    dict = malloc(sizeof(char *) * TAMANHO_BYTE);
    int i;

    for (i = 0; i < TAMANHO_BYTE; i++)
    {
        dict[i] = calloc(colunas, sizeof(char));
    }

    return dict;
}

// gerar o dicionario dos bits
void gerar_dict(char **dict, struct No *raiz, char *caminho, int colunas)
{
    char esquerda[colunas], direita[colunas];
    if (raiz->esq == NULL && raiz->dir == NULL)
    {
        strcpy(dict[raiz->byte], caminho);
    }
    else
    {
        strcpy(esquerda, caminho);
        strcpy(direita, caminho);

        strcat(esquerda, "0");
        strcat(direita, "1");

        gerar_dict(dict, raiz->esq, esquerda, colunas);
        gerar_dict(dict, raiz->dir, direita, colunas);
    }
}

// imprimir dicionario
void imprime_dict(char **dict)
{
    int i;
    printf("\n\tDicionario: \n");
    for (i = 0; i < TAMANHO_BYTE; i++)
    {
        if (strlen(dict[i]) > 0)
        {
            printf("\t%3d: %s\n", i, dict[i]);
        }
    }
}

// Função para calcular o tamanho do arquivo em bits
int calcula_tamanho_string(FILE *file)
{
    int tam_bytes = 0; // Tamanho em bytes
    unsigned char byte;
    while (fread(&byte, 1, 1, file))
    {
        tam_bytes++;
    }
    rewind(file);         // Reposiciona o ponteiro do arquivo para o início
    return tam_bytes * 8; // Retorna o tamanho em bits
}

char *codificar(char **dict, FILE *file)
{
    int tam = calcula_tamanho_string(file);       // Calcula o tamanho total do arquivo em bits
    char *codigo = calloc(tam + 1, sizeof(char)); // Aloca memória para a string codificada
    if (!codigo)
    {
        perror("Erro ao alocar memória para codificação");
        exit(EXIT_FAILURE);
    }

    char buffer[TAMANHO_VERIFICACAO]; // Buffer para armazenar o byte lido
    unsigned char byte;
    codigo[0] = '\0'; // Inicializa a string como vazia

    // Lê o arquivo byte a byte e vai concatenando a codificação
    while (fread(&byte, 1, 1, file))
    {
        strcat(codigo, dict[byte]); // Concatena o código binário do byte
    }

    rewind(file);  // Reposiciona o ponteiro do arquivo para o início
    return codigo; // Retorna o código codificado
}

// Função para verificar se é um caractere de controle
int eh_caracter_controle(int byte)
{
    return  byte < 32;
}

// Função para decodificar o código de bits usando a árvore de Huffman
char *decodificar(char *codificado, struct No *raiz)
{
    int i = 0;
    struct No *aux = raiz;
    char temp[5];                                                      // Para armazenar o valor numérico dos caracteres de controle
    char *decodificado = calloc(strlen(codificado) * 2, sizeof(char)); // Tamanho ajustado para evitar problemas de memória

    while (codificado[i] != '\0')
    {
        // Percorrendo a árvore de Huffman com base nos bits do codificado
        if (codificado[i] == '0')
        {
            aux = aux->esq;
        }
        else
        {
            aux = aux->dir;
        }

        // Se o nó atual for uma folha, o byte decodificado é encontrado
        if (aux->esq == NULL && aux->dir == NULL)
        {
            if (eh_caracter_controle(aux->byte))
            {
                // Se for um caractere de controle, converta para o valor numérico
                sprintf(temp, "%d", aux->byte); // Exibe como [valor] para caracteres de controle
                strcat(decodificado, temp);
            }
            else
            {
                // Se não for caractere de controle, simplesmente concatena como caractere
                temp[0] = aux->byte;
                temp[1] = '\0';
                strcat(decodificado, temp);
            }

            // Volta para a raiz para continuar a decodificação
            aux = raiz;
        }
        i++;
    }

    return decodificado;
}

void remover_sufixo(char *str)
{
    // Verifica se a string termina com ".txt"
    if (strstr(str, ".txt") == (str + strlen(str) - 4))
    {
        str[strlen(str) - 4] = '\0'; // Remove ".txt"
    }
    // Verifica se a string termina com ".bin"
    else if (strstr(str, ".bin") == (str + strlen(str) - 4))
    {
        str[strlen(str) - 4] = '\0'; // Remove ".bin"
    }
}

void adicionar_sufixo(char *str, const char *sufixo)
{
    // Verifica se há espaço suficiente para o sufixo
    if (strlen(str) + strlen(sufixo) + 1 <= 50)
    {                        // 50 é o tamanho máximo da string
        strcat(str, sufixo); // Adiciona o sufixo ao final da string
    }
    else
    {
        printf("Erro: espaço insuficiente para adicionar o sufixo.\n");
    }
}

// compactar e descompactar arquivo
void compactar(char *codificado, char* nome_arquivo)
{
    char *nome_arquivo_compactado = nome_arquivo;
    remover_sufixo(nome_arquivo_compactado);
    adicionar_sufixo(nome_arquivo_compactado, ".cmp");

    FILE *arquivo = fopen(nome_arquivo_compactado, "wb");
    int i = 0, j = 7;
    unsigned char mascara, byte = 0; // 1 byte = 8 bits

    if (arquivo)
    {
        while (codificado[i] != '\0')
        {
            mascara = 1;
            if (codificado[i] == '1')
            {
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if (j < 0) // tem um byte formado
            {
                fwrite(&byte, sizeof(unsigned char), 1, arquivo);
                byte = 0;
                j = 7;
            }

            i++;
        }

        if (j != 7) // se tiver um byte pela metade(se for multiplo de 8)
        {
            fwrite(&byte, sizeof(unsigned char), 1, arquivo);
        }

        fclose(arquivo);
    }
    else
    {
        printf("\nErro ao abrir/criar arquivo em compactar\n");
    }
}

unsigned int eh_bit_um(unsigned char byte, int i)
{
    unsigned char mascara = (1 << i); // 00000001
    return byte & mascara;
}

void descompactar(struct No *raiz, char *nome_arquivo)
{
    // Abrir o arquivo compactado para leitura
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo)
    {
        printf("\nErro ao abrir arquivo em descompactar\n");
        return;
    }

    // Criar nome do arquivo de saída descompactado
    char nome_arquivo_descompactado[100]; // Buffer de tamanho ajustado
    strcpy(nome_arquivo_descompactado, nome_arquivo);

    // Remover a extensão original do arquivo compactado
    remover_sufixo(nome_arquivo_descompactado);

    adicionar_sufixo(nome_arquivo_descompactado, "."); // Adiciona sufixo ".bin"

    // Abrir o arquivo de saída descompactado para escrita em modo binário
    FILE *arquivo_saida = fopen(nome_arquivo_descompactado, "wb");
    if (!arquivo_saida)
    {
        printf("\nErro ao criar arquivo de saída\n");
        fclose(arquivo); // Fecha o arquivo de entrada
        return;
    }

    struct No *aux = raiz;
    unsigned char byte;
    int i;
    char temp[5]; // Buffer para caracteres de controle

    // Lê os bytes do arquivo compactado
    while (fread(&byte, sizeof(unsigned char), 1, arquivo))
    {
        for (i = 7; i >= 0; i--) // Percorre cada bit do byte
        {
            if (eh_bit_um(byte, i))
            {
                aux = aux->dir;
            }
            else
            {
                aux = aux->esq;
            }

            // Se encontrou uma folha na árvore, decodifica o caractere
            if (aux->esq == NULL && aux->dir == NULL)
            {
                if (eh_caracter_controle(aux->byte))
                {
                    // Se for um caractere de controle, exibe o valor numérico entre colchetes
                    sprintf(temp, "%d", aux->byte);
                    fwrite(temp, sizeof(char), strlen(temp), arquivo_saida); // Grava o valor numérico no arquivo
                }
                else
                {
                    // Caso contrário, grava o byte decodificado no arquivo
                    fwrite(&aux->byte, sizeof(unsigned char), 1, arquivo_saida);
                }

                aux = raiz; // Volta para a raiz para decodificar o próximo caractere
            }
        }
    }

    // Fecha os arquivos de entrada e saída
    fclose(arquivo);
    fclose(arquivo_saida);

    printf("Arquivo descompactado salvo como: %s\n", nome_arquivo_descompactado);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <tipo> <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arquivo_entrada = fopen(argv[2], "rb");
    if (!arquivo_entrada)
    {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    if(*argv[1] == 'c')
    {
        int frequencias[256] = {0};
        calcular_frequencias(arquivo_entrada, frequencias);

        printf("TABELA DE FREQUENCIA: \n");
        exibir_frequencia(frequencias);

        printf("Construindo heap...\n");
        struct HeapMin *heap = criar_heap(TAMANHO_HEAP);
        construirHeap(heap, frequencias, TAMANHO_BYTE);
        printf("Heap construido com sucesso...\n");

        printf("TABELA Heap(lista de prioridade): \n");
        heap_sort(heap);
        exibir_heap(heap);

        printf("Construindo arvore de huffman: \n");
        montar_arvore(heap);
        printf("Arvore construida com sucesso...\n");

        printf("Arvore de huffman: \n");
        imprimir_arvore(heap->arvores[0].raiz);

        printf("Construindo o dicionario dos codigos: \n");
        int colunas = altura_arvore(heap->arvores[0].raiz) + 1;
        char **dict = aloca_dict(colunas);
        gerar_dict(dict, heap->arvores[0].raiz, "", colunas);
        printf("Dicionario dos codigos: \n");
        imprime_dict(dict);

        printf("tamanho do arquivo(numero de bits): %d\n", calcula_tamanho_string(arquivo_entrada));

        char *codificado = codificar(dict, arquivo_entrada);
        printf("Codificado(string): %s \n", codificado);
        char *decodificado = decodificar(codificado, heap->arvores[0].raiz);
        printf("Decodificado: %s\n", decodificado);
        printf("Compactando... \n");
        compactar(codificado, argv[2]);
        printf("Compactado com sucesso!\n");

        /*
          * descompactação para arquivos .txt funciona perfeitamente, porém nao tem a serialização
          * descompactação para arquivos .bin nao esta totalmente correta, nao deu tempo de corrigir
          * nao implementado a serialização da arvore e arquivo compactado
        */
       // não deu pra terminar :(

        // liberando dict
        for (int i = 0; i < TAMANHO_BYTE; i++)
        {
            free(dict[i]);
        }

        free(dict);

        free(codificado);
        free(decodificado);
        liberar_heap(heap);
        fclose(arquivo_entrada);
    }
    else
    {
        // printf("Descompactando...\n");
        // descompactar(heap->arvores[0].raiz, "teste.cmp");
        //printf("\n");
        //printf("Descompactado com sucesso!\n");
        //printf("\n");
    }

    return 0;
}