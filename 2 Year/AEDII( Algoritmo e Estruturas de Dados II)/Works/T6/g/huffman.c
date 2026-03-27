#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAMANHO_MAX_NOME_ARQUIVO 256

#define TAM 256

// Estrutura do nó da árvore de Huffman
struct No
{
  unsigned char caractere;
  unsigned int frequencia;
  struct No *esq, *dir;
};

// Estrutura da lista de nós (para construir a árvore)
struct Lista
{
  struct No *no;
  struct Lista *prox;
};

// Função para criar um nó de Huffman
struct No *criar_no(unsigned char caractere, unsigned int frequencia)
{
  struct No *no = (struct No *)malloc(sizeof(struct No));
  no->caractere = caractere;
  no->frequencia = frequencia;
  no->esq = no->dir = NULL;
  return no;
}

// Função para inserir um nó na lista de nós
void inserir_lista(struct Lista **lista, struct No *no)
{
  struct Lista *novo = (struct Lista *)malloc(sizeof(struct Lista));
  novo->no = no;
  novo->prox = NULL;

  if (*lista == NULL || (*lista)->no->frequencia > no->frequencia)
  {
    novo->prox = *lista;
    *lista = novo;
  }
  else
  {
    struct Lista *temp = *lista;
    while (temp->prox != NULL && temp->prox->no->frequencia <= no->frequencia)
    {
      temp = temp->prox;
    }
    novo->prox = temp->prox;
    temp->prox = novo;
  }
}

// Função para remover o primeiro nó da lista
struct No *remover_lista(struct Lista **lista)
{
  if (*lista == NULL)
    return NULL;
  struct Lista *temp = *lista;
  *lista = (*lista)->prox;
  struct No *no = temp->no;
  free(temp);
  return no;
}

// Função para construir a árvore de Huffman a partir da lista de nós
struct No *construir_arvore(struct Lista **lista)
{
  while (*lista && (*lista)->prox)
  {
    struct No *esq = remover_lista(lista);
    struct No *dir = remover_lista(lista);
    struct No *novo_no = criar_no(0, esq->frequencia + dir->frequencia);
    novo_no->esq = esq;
    novo_no->dir = dir;
    inserir_lista(lista, novo_no);
  }
  return (*lista)->no;
}

// Função para gerar o código de Huffman para cada caractere
void gerar_codigo(struct No *raiz, char **codigos, char *codigo_atual, int profundidade)
{
  if (raiz->esq == NULL && raiz->dir == NULL)
  {
    codigo_atual[profundidade] = '\0';
    codigos[raiz->caractere] = strdup(codigo_atual);
  }
  else
  {
    codigo_atual[profundidade] = '0';
    gerar_codigo(raiz->esq, codigos, codigo_atual, profundidade + 1);
    codigo_atual[profundidade] = '1';
    gerar_codigo(raiz->dir, codigos, codigo_atual, profundidade + 1);
  }
}

// Função para gravar a árvore de Huffman no arquivo de saída
void gravar_arvore(struct No *raiz, FILE *saida)
{
  if (raiz->esq == NULL && raiz->dir == NULL)
  {
    fputc('1', saida);
    fputc(raiz->caractere, saida);
  }
  else
  {
    fputc('0', saida);
    gravar_arvore(raiz->esq, saida);
    gravar_arvore(raiz->dir, saida);
  }
}

// Função para ler a árvore de Huffman do arquivo de entrada
struct No *ler_arvore(FILE *entrada)
{
  int bit = fgetc(entrada);
  if (bit == '1')
  {
    unsigned char caractere = fgetc(entrada);
    return criar_no(caractere, 0);
  }
  else
  {
    struct No *no = criar_no(0, 0);
    no->esq = ler_arvore(entrada);
    no->dir = ler_arvore(entrada);
    return no;
  }
}

// Função para compactar um arquivo
void compactar(const char *entrada_nome, const char *saida_nome)
{
  FILE *entrada = fopen(entrada_nome, "rb");
  if (!entrada)
  {
    printf("Erro ao abrir o arquivo de entrada.\n");
    exit(1);
  }

  // Contagem da frequência dos caracteres
  unsigned int frequencias[TAM] = {0};
  int c;
  while ((c = fgetc(entrada)) != EOF)
  {
    frequencias[(unsigned char)c]++;
  }

  // Criar lista de nós e gerar árvore de Huffman
  struct Lista *lista = NULL;
  for (int i = 0; i < TAM; i++)
  {
    if (frequencias[i] > 0)
    {
      inserir_lista(&lista, criar_no(i, frequencias[i]));
    }
  }
  struct No *arvore = construir_arvore(&lista);

  // Gerar código de Huffman para cada caractere
  char *codigos[TAM] = {NULL};
  char codigo_atual[TAM];
  gerar_codigo(arvore, codigos, codigo_atual, 0);

  // Abrir arquivo de saída
  FILE *saida = fopen(saida_nome, "wb");
  if (!saida)
  {
    printf("Erro ao abrir o arquivo de saída.\n");
    exit(1);
  }

  // Gravar a árvore no arquivo
  gravar_arvore(arvore, saida);

  // Compactar os dados
  rewind(entrada);
  unsigned char buffer = 0;
  int pos = 0;
  while ((c = fgetc(entrada)) != EOF)
  {
    char *codigo = codigos[(unsigned char)c];
    for (int i = 0; codigo[i] != '\0'; i++)
    {
      if (codigo[i] == '1')
      {
        buffer |= (1 << (7 - pos));
      }
      pos++;
      if (pos == 8)
      {
        fputc(buffer, saida);
        buffer = 0;
        pos = 0;
      }
    }
  }
  if (pos > 0)
  {
    fputc(buffer, saida);
  }

  fclose(entrada);
  fclose(saida);
  printf("Arquivo compactado com sucesso: %s\n", saida_nome);
}

// Função para descompactar um arquivo
void descompactar(const char *entrada_nome, const char *saida_nome)
{
  FILE *entrada = fopen(entrada_nome, "rb");
  if (!entrada)
  {
    printf("Erro ao abrir o arquivo de entrada.\n");
    exit(1);
  }

  // Ler a árvore de Huffman
  struct No *arvore = ler_arvore(entrada);

  // Abrir o arquivo de saída
  FILE *saida = fopen(saida_nome, "wb");
  if (!saida)
  {
    printf("Erro ao abrir o arquivo de saída.\n");
    exit(1);
  }

  // Descompactar os dados
  struct No *no = arvore;
  int c;
  while ((c = fgetc(entrada)) != EOF)
  {
    for (int i = 7; i >= 0; i--)
    {
      if (c & (1 << i))
      {
        no = no->dir;
      }
      else
      {
        no = no->esq;
      }

      if (no->esq == NULL && no->dir == NULL)
      {
        fputc(no->caractere, saida);
        no = arvore;
      }
    }
  }

  fclose(entrada);
  fclose(saida);
  printf("Arquivo descompactado com sucesso: %s\n", saida_nome);
}

// Função para remover a extensão de um arquivo
void remover_extensao(char *nome_arquivo)
{
  char *ponto = strrchr(nome_arquivo, '.');
  if (ponto != NULL)
  {
    *ponto = '\0'; // Remove a extensão ao colocar o terminador de string
  }
}

// Função para gerar o nome do arquivo compactado (adiciona .cmp)
void gerar_nome_arquivo_compactado(char *nome_arquivo_entrada, char *nome_arquivo_saida)
{
  strcpy(nome_arquivo_saida, nome_arquivo_entrada);
  remover_extensao(nome_arquivo_saida); // Remove a extensão original
  strcat(nome_arquivo_saida, ".cmp");   // Adiciona a extensão .cmp
}

// Função para gerar o nome do arquivo descompactado (remove .cmp e restaura a extensão original)
void gerar_nome_arquivo_descompactado(char *nome_arquivo_entrada, char *nome_arquivo_saida)
{
  strcpy(nome_arquivo_saida, nome_arquivo_entrada);
  char *extensao_cmp = strstr(nome_arquivo_saida, ".cmp");
  if (extensao_cmp != NULL)
  {
    *extensao_cmp = '\0'; // Remove a extensão .cmp
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Uso: %s <c|d> <nome do arquivo>\n", argv[0]);
    return 1;
  }

  char opcao = argv[1][0];
  char *nome_arquivo_entrada = argv[2];
  char nome_arquivo_saida[TAMANHO_MAX_NOME_ARQUIVO];

  if (opcao == 'c')
  {
    // Compactar o arquivo
    gerar_nome_arquivo_compactado(nome_arquivo_entrada, nome_arquivo_saida);
    printf("Compactando o arquivo '%s' para '%s'\n", nome_arquivo_entrada, nome_arquivo_saida);
    compactar(nome_arquivo_entrada, nome_arquivo_saida);
  }
  else if (opcao == 'd')
  {
    // Descompactar o arquivo
    gerar_nome_arquivo_descompactado(nome_arquivo_entrada, nome_arquivo_saida);
    printf("Descompactando o arquivo '%s' para '%s'\n", nome_arquivo_entrada, nome_arquivo_saida);
    descompactar(nome_arquivo_entrada, nome_arquivo_saida);
  }
  else
  {
    printf("Opção inválida! Use 'c' para compactar ou 'd' para descompactar.\n");
    return 1;
  }

  return 0;
}