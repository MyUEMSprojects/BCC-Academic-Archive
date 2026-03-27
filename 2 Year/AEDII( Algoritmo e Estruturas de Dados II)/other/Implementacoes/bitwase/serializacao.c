#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct No
{
    int8_t chave;
    struct No *esq, *dir;
};

struct serializacao
{
    struct No *arvore; // Mudando para ponteiro para facilitar a serialização da árvore
    unsigned char *dados_binarios;
    size_t tamanho_dados;
};

// Função para serializar a árvore binária em pré-ordem
void serializar_arvore(FILE *arquivo, struct No *no)
{
    if (no == NULL)
    {
        int8_t marcador = -1; // Marcador para NULL
        fwrite(&marcador, sizeof(int8_t), 1, arquivo);
        return;
    }

    fwrite(&no->chave, sizeof(int8_t), 1, arquivo); // Escreve a chave do nó
    serializar_arvore(arquivo, no->esq);            // Serializa a subárvore esquerda
    serializar_arvore(arquivo, no->dir);            // Serializa a subárvore direita
}

// Função para desserializar a árvore binária de um arquivo
struct No *desserializar_arvore(FILE *arquivo)
{
    int8_t chave;
    fread(&chave, sizeof(int8_t), 1, arquivo);

    if (chave == -1)
    {
        return NULL; // Nó nulo
    }

    struct No *no = (struct No *)malloc(sizeof(struct No));
    no->chave = chave;
    no->esq = desserializar_arvore(arquivo); // Desserializa a subárvore esquerda
    no->dir = desserializar_arvore(arquivo); // Desserializa a subárvore direita

    return no;
}

// Função para serializar a estrutura "serializacao"
void serializar_struct(const char *nome_arquivo, struct serializacao *dados)
{
    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }

    // Serializa a árvore binária
    serializar_arvore(arquivo, dados->arvore);

    // Serializa os dados binários e seu tamanho
    fwrite(&dados->tamanho_dados, sizeof(size_t), 1, arquivo);
    fwrite(dados->dados_binarios, sizeof(unsigned char), dados->tamanho_dados, arquivo);

    fclose(arquivo);
}

// Função para desserializar a estrutura "serializacao"
void desserializar_struct(const char *nome_arquivo, struct serializacao *dados)
{
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return;
    }

    // Desserializa a árvore binária
    dados->arvore = desserializar_arvore(arquivo);

    // Desserializa os dados binários
    fread(&dados->tamanho_dados, sizeof(size_t), 1, arquivo);
    dados->dados_binarios = (unsigned char *)malloc(dados->tamanho_dados * sizeof(unsigned char));
    fread(dados->dados_binarios, sizeof(unsigned char), dados->tamanho_dados, arquivo);

    fclose(arquivo);
}

// Função auxiliar para criar um novo nó
struct No *criar_no(int8_t chave)
{
    struct No *no = (struct No *)malloc(sizeof(struct No));
    no->chave = chave;
    no->esq = no->dir = NULL;
    return no;
}

// Função auxiliar para liberar a memória da árvore
void liberar_arvore(struct No *no)
{
    if (no == NULL)
        return;
    liberar_arvore(no->esq);
    liberar_arvore(no->dir);
    free(no);
}

int main()
{
    // Criando uma árvore binária simples
    struct No *raiz = criar_no(10);
    raiz->esq = criar_no(5);
    raiz->dir = criar_no(20);

    // Definindo os dados binários (exemplo)
    unsigned char dados[] = {0xDE, 0xAD, 0xBE, 0xEF}; // Exemplo de dados binários

    // Estrutura de serialização
    struct serializacao dados_serialize;
    dados_serialize.arvore = raiz;
    dados_serialize.dados_binarios = dados;
    dados_serialize.tamanho_dados = sizeof(dados) / sizeof(unsigned char);

    // Serializando a estrutura
    serializar_struct("saida.bin", &dados_serialize);

    // Limpar a memória e carregar novamente os dados serializados
    liberar_arvore(raiz);

    // Estrutura para armazenar os dados desserializados
    struct serializacao dados_deserialize;

    // Desserializando a estrutura
    desserializar_struct("saida.bin", &dados_deserialize);

    // Exibindo os dados desserializados
    printf("Chave da raiz desserializada: %d\n", dados_deserialize.arvore->chave);
    printf("Tamanho dos dados binários: %zu\n", dados_deserialize.tamanho_dados);
    printf("Dados binários: ");
    
    for (size_t i = 0; i < dados_deserialize.tamanho_dados; ++i)
    {
        printf("0x%X ", dados_deserialize.dados_binarios[i]);
    }
    printf("\n");

    // Limpar a memória alocada
    liberar_arvore(dados_deserialize.arvore);
    free(dados_deserialize.dados_binarios);

    return 0;
}