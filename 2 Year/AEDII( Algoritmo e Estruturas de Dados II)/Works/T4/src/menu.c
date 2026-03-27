#include "menu.h"

int mainMenu(int option, struct s_arvore **tree)
{
    do
    {
        printf("\n\t1 - Busca\n\t2 - Inserir\n\t3 - Remover\n\t4 - Ler arquivo binario\n\t5 - Escrever arquivo binario\n\t6 - Limpar Arvore\n\t7 - Sair\n");
        scanf("%d", &option);
    } while (option < 1 || option > 7);

    switch (option)
    {
    case 1:
    {
        int32_t x;
        printf("\n\tDigite o valor da chave a buscado: ");
        scanf("%d", &x);
        
        struct s_no* node_founded = search((*tree)->raiz, x);
        if(node_founded != NULL)
        {
           printf("\n\tNo de valor %d encontrado!\n", node_founded->chave);
        }
        else
        {
            printf("\n\tNo nao encontrado!\n");
        }
        break;
    }
    case 2:
    {
        int32_t x;
        bool h = false;
        printf("\n\tDigite o valor da chave a ser inserida: ");
        scanf("%d", &x);
        insert(&(*tree)->raiz, x, &h);
        break;
    }
    case 3:
    {
        int32_t x;
        bool h = false;
        printf("\n\tDigite o valor da chave a ser deletada: ");
        scanf("%d", &x);
        struct s_no *node_to_delete = search((*tree)->raiz, x);
        tree_delete(&(*tree)->raiz, node_to_delete, &h);
        break;
    }
    case 4:
    {
        char filename[100];
        printf("Digite o nome do arquivo binario a ser lido: ");
        scanf("%99s", filename);

        (*tree) = ReadFile(filename);

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