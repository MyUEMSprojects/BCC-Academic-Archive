#include "menu.h"

int main()
{
    int option = 0;
    struct s_arvore *tree = initTree();

    gfx_init(WIDTH, HEIGHT, "Visualização");

    while (option != 7)
    {
        option = mainMenu(option, &tree);
    }

    gfx_quit();
    deallocateTree(tree->raiz);
    free(tree);
    
    return 0;
}