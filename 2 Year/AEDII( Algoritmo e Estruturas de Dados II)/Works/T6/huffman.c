#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 256

struct No
{
  unsigned char c;
  int frequencia;
  struct No *esq, *dir, *prox;
};

struct Lista
{
  struct No *inicio;
  int tam;
};


void inicializar_tabela_com_zero(unsigned int tab[])
{
  for (int i = 0; i < TAM; i++)
       tab[i] = 0;
}

void preenche_tab_frequencia(unsigned char texto[], unsigned int tab[])
{
  int i = 0;
  while (texto[i] != '\0')
  {
    tab[texto[i]]++;
    i++;
  }
}

void imprime_tab_frequencia(unsigned int tab[])
{
  int i;
  
  printf("\tTABELA DE FREQUENCIA\n");
  for(i = 0; i < TAM; i++)
  {
    if(tab[i] > 0)
       printf("\t%d = %u = %c\n", i, tab[i], i);
  }
}

void criar_lista(struct Lista *lista)
{
  lista->inicio = NULL;
  lista->tam = 0;
}

void inserir_ordenado(struct Lista *lista, struct No *no)
{
  if(lista->inicio == NULL)
  {
    lista->inicio = no;
  }
  // tem frequencia menor q o inicio da lista
  else if (no->frequencia < lista->inicio->frequencia)
  {
    no->prox = lista->inicio;
    lista->inicio = no;
  }
  else
  {
    struct No* aux;

    aux = lista->inicio;
    while (aux->prox && aux->prox->frequencia <= no->frequencia)
          aux = aux->prox;
    no->prox = aux->prox;
    aux->prox = no;
  }

  lista->tam++;
}

void preencher_lista(unsigned int tab[], struct Lista *lista)
{
  int i;
  struct No *novo;
  for(i = 0; i < TAM; i++)
  {
    if(tab[i] > 0)
    {
      novo = (struct No*) malloc(sizeof(struct No));

      if(novo)
      {
        novo->c = i;
        novo->frequencia = tab[i];
        novo->dir = NULL;
        novo->esq = NULL;
        novo->prox = NULL;

        inserir_ordenado(lista, novo);
      }
      else
      {
        printf("\tErro ao alocar memória em preencher_lista!\n");
        break; 
      }
    }
  }
}

void imprimir_lista(struct Lista* lista)
{
  struct No* aux = lista->inicio;
   
  printf("\tLista ordenada: Tamanho %d\n", lista->tam);
  while (aux)
  {
    printf("\tCaractere: %c Frequencia: %d\n", aux->c, aux->frequencia);
    aux = aux->prox;
  }
  
}

struct No* remove_no_inicio(struct Lista* lista)
{
  struct No* aux = NULL;

  if(lista->inicio)
  {
    aux = lista->inicio;
    lista->inicio = aux->prox;
    aux->prox = NULL;
    lista->tam--;
  } 
  
  return aux;
}

struct No* montar_arvore(struct Lista* lista)
{

  struct No *primeiro, *segundo, *novo;
  while (lista->tam > 1)
  {
    primeiro = remove_no_inicio(lista);
    segundo = remove_no_inicio(lista);
    novo = (struct No*) malloc(sizeof(struct No));

    if(novo)
    {
      novo->c = '+';
      novo->frequencia = primeiro->frequencia + segundo->frequencia;
      novo->esq = primeiro;
      novo->dir = segundo;
      novo->prox = NULL;

      inserir_ordenado(lista, novo);
    }
    else
    {
      printf("\n\tErro ao alocar memória em montar_arvore!\n");
      break;
    }
  }

  return lista->inicio;
}

void imprimir_arvore(struct No* raiz, int tam)
{
  if(raiz->esq == NULL &&  raiz->dir == NULL)
  {
    printf("\tFolha: %c\tAltura: %d\n", raiz->c, tam);
  }
  else
  {
    imprimir_arvore(raiz->esq, tam + 1);
    imprimir_arvore(raiz->dir, tam + 1);
  }
}

int altura_arvore(struct No* raiz)
{
  int esq, dir;
  if(raiz == NULL)
  {
    return -1;
  }
  else
  {
    esq = altura_arvore(raiz->esq) + 1;
    dir = altura_arvore(raiz->dir) + 1;


    if(esq > dir)
    {
      return esq;
    }
    else
    {
      return dir;
    }
  }
}

char** aloca_dicionario(int colunas)
{

  char **dicionario;
  dicionario = malloc(sizeof(char *) * TAM);
  int i;

  for(i = 0; i < TAM;i++)
      dicionario[i] = calloc(colunas, sizeof(char));
  
  return dicionario;
}

void gerar_dicionario(char **dicionario, struct No *raiz, char *caminho, int colunas)
{
  char esquerda[colunas], direita[colunas];
  if(raiz->esq == NULL && raiz->dir == NULL)
  {
    strcpy(dicionario[raiz->c], caminho);
  }
  else
  {
    strcpy(esquerda, caminho);
    strcpy(direita, caminho);

    strcat(esquerda, "0");
    strcat(direita, "1");

    gerar_dicionario(dicionario, raiz->esq, esquerda, colunas);
    gerar_dicionario(dicionario, raiz->dir, direita, colunas);
  }
}

void imprime_dicionario(char **dicionario)
{
  int i;
  printf("\n\tDicionario: \n");
  for(i = 0; i < TAM; i++)
  {
    if(strlen(dicionario[i]) > 0)
       printf("\t%3d: %s\n", i, dicionario[i]);
  }
}

int calcula_tamanho_string(char **dicionario, unsigned char *texto)
{
  int i = 0, tam = 0;
  while (texto[i] != '\0')
  {
    tam = tam + strlen(dicionario[texto[i]]);
    i++;
  }
  
  return tam + 1;
}

char* codificar(char **dicionario, unsigned char *texto)
{
  int i = 0, tam = calcula_tamanho_string(dicionario, texto);

  char *codigo = calloc(tam, sizeof(char));

  while (texto[i] != '\0')
  {
    strcat(codigo, dicionario[texto[i]]);
    i++;
  }
  
  return codigo;
}

char* decodificar(unsigned char texto[], struct No* raiz)
{
  int i = 0;
  struct No* aux = raiz;
  char temp[2];
  char *decodificado = calloc(strlen(texto), sizeof(char));
  while (texto[i] != '\0')
  {
    if(texto[i] == '0')
        aux = aux->esq;
    else
       aux = aux->dir;
    
    if(aux->esq == NULL && aux->dir == NULL)
    {
      temp[0] = aux->c;
      temp[1] = '\0';
      strcat(decodificado, temp);
      aux = raiz;
    }

    i++;
  }
  
  return decodificado;
}

void compactar(unsigned char str[])
{
  FILE *arquivo = fopen("compactado.dcmp", "wb");
  int i = 0, j = 7;
  unsigned char mascara, byte = 0;  // 1 byte = 8 bits

  if(arquivo)
  {
    while (str[i] != '\0')
    {
      mascara = 1;
      if(str[i] == '1')
      {
        mascara = mascara << j;
        byte = byte | mascara;
      }
      j--;
      
      if(j < 0) // tem um byte formado
      {
        fwrite(&byte, sizeof(unsigned char), 1, arquivo);
        byte = 0;
        j = 7;
      }

      i++;
    }

    if(j != 7) // se tiver um byte pela metade(se for multiplo de 8)
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

void descompactar(struct No* raiz)
{
  FILE *arquivo = fopen("compactado.dcmp", "rb");
  struct No *aux = raiz;
  unsigned char byte; // 10110111
  int i;

  if(arquivo)
  {
    while (fread(&byte, sizeof(unsigned char), 1, arquivo))
    {
      for(i = 7; i >= 0; i--)
      {
        if(eh_bit_um(byte, i))
        {
          aux = aux->dir;
        }
        else
        {
          aux = aux->esq;
        }

        if(aux->esq == NULL && aux->dir == NULL)
        {
          printf("%c", aux->c);
          aux = raiz;
        }
      }    
    }

    fclose(arquivo);
  }
  else
  {
    printf("\nErro ao abrir arquivo em descompactar\n");
  }
}

int descobrir_tamanho()
{
  FILE *arq = fopen("teste.txt", "r");
  int tam = 0;

  if(arq)
  {
    while (fgetc(arq) != -1)
    {
      tam++;
    }

    fclose(arq);
  }
  else
  {
    printf("\nErro ao abrir arquivo em descobrir_tamanho\n");
  }

  return tam;
}

void ler_texto(unsigned char *texto)
{
  FILE *arq = fopen("sla.txt", "r");
  char letra;
  int i = 0;

  if (arq)
  {
    while (!feof(arq))
    {
      letra = fgetc(arq);
      if(letra != -1)
      {
        texto[i] = letra;
        i++;
      }    
    }
    fclose(arq);
  }
  else
  {
    printf("\nErro ao abrir arquivo em ler_texto\n");
  }
}

int main()
{
  // unsigned char texto[] = "Vamos aprender a programar";
  unsigned char *texto;
  unsigned int tabela_frequencia[TAM];
  struct Lista lista;
  struct No* arvore;
  int colunas, tam;
  char** dicionario;
  char *codificado, *decodificadoo;

  tam = descobrir_tamanho();
  printf("\nQuantidade: %d\n", tam);

  texto = calloc(tam + 2, sizeof(unsigned char));
  ler_texto(texto);
  printf("\nTEXTO:\n%s\n", texto);

  inicializar_tabela_com_zero(tabela_frequencia);
  preenche_tab_frequencia(texto, tabela_frequencia);
  imprime_tab_frequencia(tabela_frequencia);

  criar_lista(&lista);
  preencher_lista(tabela_frequencia, &lista);
  imprimir_lista(&lista);

  arvore = montar_arvore(&lista);
  printf("\n\tArvore de Huffman\n");
  imprimir_arvore(arvore, 0);

  colunas = altura_arvore(arvore) + 1;
  dicionario = aloca_dicionario(colunas);
  gerar_dicionario(dicionario, arvore, "", colunas);
  imprime_dicionario(dicionario);
  
  // -----------------------------------------------

  codificado = codificar(dicionario, texto);
  printf("\n\tTexto codificado: %s\n", codificado);

  // -----------------------------------------------

  decodificadoo = decodificar(codificado, arvore);
  printf("\n\tTexto decodificado: %s\n", decodificadoo);

  compactar(codificado);

  printf("\nARQUIVO DESCOMPACTADO!\n");
  descompactar(arvore);
  printf("\n\n");
  

  free(texto);
  free(codificado);
  free(decodificadoo);

  // desalocar arvore e dicionario, porém nao é necessario para esse programa
  return 0;
}