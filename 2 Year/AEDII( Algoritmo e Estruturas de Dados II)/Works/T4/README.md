# OBS:
## o programa compilado esta na pasta build, "main.exe"!
## o programa nao deve estar em um diretório com "espaços" entre as palavras, senão da erro no makefile
## O programa usou como base os algoritmos passados na aula e os algoritmos dos livros.
## Livros:
# Algoritmos - algoritmos-teoria-e-prc3a1tica-3ed-thomas-cormen
# Szwarcfiter, Jayme Luiz - Estruturas De Dados E Seus Algoritmos (2010)

# Trabalho:

Faça um programa em C para Linux que suporte as seguintes operações sobre árvores AVL:

    Busca, inserção e remoção de chaves inteiras: quando necessário, devem ser realizadas as rotações adequadas. Serão aceitos apenas os algoritmos apresentados/discutidos em sala de aula;
    Visualização da árvore em modo gráfico usando a biblioteca gfx. Deve ser impressa a representação hierárquica contendo  a chave e o balanço de cada nó;
    Leitura e gravação da árvore em arquivo formato binário usando percurso em pré-ordem. A árvore não deve ser reconstruída através de operações de inserção;
    Desalocação da árvore.

Deve ser apresentada uma interface (modo texto ou gráfico) de simples uso que suporte todas as operações solicitadas.

A seguinte estrutura de dados deve ser utilizada para representar na memória cada nó da árvore:

struct s_no

{

    int32_t chave:28;

    int32_t bal:2;

    int32_t reservado:2; /* sem uso */

    struct s_no* esq;

    struct s_no* dir;

};

A seguinte estrutura de dados deve ser utilizada para representar em arquivo cada nó da árvore:

struct s_arq_no

{

    int32_t chave:28;

    int32_t bal:2;

    uint32_t esq:1;

    uint32_t dir:1;

};

O código deve estar devidamente organizado, identado, comentado e sem warnings  usando  as  opções -Wall -Wextra -pedantic do gcc.
