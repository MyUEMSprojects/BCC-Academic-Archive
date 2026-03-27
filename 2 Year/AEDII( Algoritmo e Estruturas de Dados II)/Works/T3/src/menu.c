#include "menu.h"

int mainMenu(int option, struct s_arvore **tree)
{
    do
    {
        printf("\n\t1 - Buscas\n\t2 - Inserir\n\t3 - Remover\n\t4 - Ler arquivo binario\n\t5 - Escrever arquivo binario\n\t6 - Limpar Arvore\n\t7 - Sair\n");
        scanf("%d", &option);
    } while (option < 1 || option > 7);

    switch (option)
    {
    case 1:
    {
        int searchOption = 0;
        while (searchOption != 6)
        {
            do
            {
                printf("\n\t1 - Buscar maior\n\t2 - Buscar menor\n\t3 - Buscar Predecessor\n\t4 - Buscar Sucessor\n\t5 - Buscar chave\n\t6 - Voltar\n");
                scanf("%d", &searchOption);
            } while (searchOption < 1 || searchOption > 6);

            switch (searchOption)
            {
            case 1:
            {
                if ((*tree)->raiz != NULL)
                {
                    struct s_no *maior = searchLarger((*tree)->raiz);
                    printf("\n\tMaior chave: %d\n", maior->chave);
                }
                else
                {
                    printf("\n\tNo não encontrado!\n");
                }
                break;
            }
            case 2:
            {
                if ((*tree)->raiz != NULL)
                {
                    struct s_no *menor = searchSmaller((*tree)->raiz);
                    printf("\n\tMenor chave: %d\n", menor->chave);
                }
                else
                {
                    printf("\n\tNo não encontrado!\n");
                }
                break;
            }
            case 3:
            {
                int32_t x;
                printf("\n\tDigite o valor para encontrar o Predecessor: ");
                scanf("%d", &x);
                if ((*tree)->raiz != NULL)
                {
                    struct s_no *Predecessor = searchPredecessor((*tree)->raiz, x);
                    if (Predecessor != NULL)
                    {
                        printf("\n\tO Predecessor do no com chave %d é o no com chave %d\n", x, Predecessor->chave);
                    }
                    else
                    {
                        printf("\n\tPredecessor não encontrado!\n");
                    }
                }
                else
                {
                    printf("\n\tÁrvore vazia!\n");
                }
                break;
            }
            case 4:
            {
                int32_t x;
                printf("\n\tDigite o valor para encontrar o Sucessor: ");
                scanf("%d", &x);
                if ((*tree)->raiz != NULL)
                {
                    struct s_no *sucessor = searchSuccessor((*tree)->raiz, x);
                    if (sucessor != NULL)
                    {
                        printf("\n\tO sucessor do no com chave %d é o no com chave %d\n", x, sucessor->chave);
                    }
                    else
                    {
                        printf("\n\tSucessor não encontrado!\n");
                    }
                }
                else
                {
                    printf("\n\tÁrvore vazia!\n");
                }
                break;
            }
            case 5:
            {
                int32_t x;
                printf("\n\tDigite o valor da chave a ser encontrada: ");
                scanf("%d", &x);
                if ((*tree)->raiz != NULL)
                {
                    struct s_no *no_encontrado = search((*tree)->raiz, x);
                    if (no_encontrado != NULL)
                    {
                        printf("\n\tNo encontrado com chave %d\n", no_encontrado->chave);
                    }
                    else
                    {
                        printf("\n\tNo não encontrado!\n");
                    }
                }
                else
                {
                    printf("\n\tÁrvore vazia!\n");
                }
                break;
            }
            case 6:
                break;
            }
        }
        break;
    }
    case 2:
    {
        int32_t x;
        printf("\n\tDigite o valor da chave a ser inserida: ");
        scanf("%d", &x);
        insert(*tree, x);
        break;
    }
    case 3:
    {
        int32_t x;
        printf("\n\tDigite o valor da chave a ser deletada: ");
        scanf("%d", &x);
        struct s_no *deletado = delete ((*tree)->raiz, x);

        if (deletado != NULL)
        {
            printf("\n\tNo deletado com sucesso!\n");
        }
        else
        {
            printf("\n\tNo não encontrado!\n");
        }

        break;
    }
    case 4:
    {
        char filename[100];
        printf("Digite o nome do arquivo binario a ser lido: ");
        scanf("%99s", filename);

        readFile(tree, filename);

        break;
    }
    case 5:
    {
        char filename[100];
        printf("Digite um nome para salvar o arquivo binario: ");
        scanf("%99s", filename);

        WriteFile((*tree)->raiz, filename);
        break;
    }
    case 6:
    {
        deallocateTree((*tree)->raiz);
        (*tree)->raiz = NULL;
        *tree = initTree();

        gfx_clear();
        gfx_paint();
        break;
    }
    case 7:
        return option;
    }

    if (option == 2 || option == 3 || option == 4)
    {
        updateTreeDisplay(*tree);
    }

    return option;
}