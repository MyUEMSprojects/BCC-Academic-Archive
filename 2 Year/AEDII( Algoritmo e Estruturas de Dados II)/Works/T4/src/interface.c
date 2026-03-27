#include "interface.h"

int calcularPosX(struct s_no *L, int level, int horizontalSpacing)
{
    if (L == NULL)
    {
        return 0;
    }

    // Calcula as posições dos nós filhos
    int leftPosX = calcularPosX(L->esq, level + 1, horizontalSpacing);
    int rightPosX = calcularPosX(L->dir, level + 1, horizontalSpacing);

    if (L->esq == NULL && L->dir == NULL)
    {
        return horizontalSpacing / 2;
    }

    // Calcula a posição atual considerando o espaçamento dos filhos
    return (leftPosX + rightPosX) / 2;
}

void drawConection(int x1, int y1, int x2, int y2)
{
    gfx_set_color(255, 165, 0);
    gfx_line(x1, y1, x2, y2);
}

void drawNode(int x, int y, int key, int bal)
{
    int size = 40;

    gfx_set_color(255, 165, 0);
    gfx_filled_rectangle(x, y, x + size, y + size);

    char buffer[20];
    sprintf(buffer, "%d", key);
    gfx_set_color(0, 0, 0);
    gfx_text(x + 10, y + 10, buffer);

    // Desenha o valor de balanceamento ao lado do nó
    sprintf(buffer, "%d", bal);
    gfx_set_color(255, 255, 255);
    gfx_text(x + size + 10, y + 20, buffer);

    gfx_set_color(0, 0, 0);
    gfx_rectangle(x, y, x + size, y + size);
}

void drawTree(struct s_no *L, int level, int posX, int posY, int horizontalSpacing, int verticalSpacing)
{
    if (L == NULL)
    {
        return;
    }

    int displacement = horizontalSpacing / 2;

    if (L->esq != NULL)
    {
        drawConection(posX + 20, posY + 40, posX - displacement + 20, posY + verticalSpacing);
        drawTree(L->esq, level + 1, posX - displacement, posY + verticalSpacing, horizontalSpacing / 2, verticalSpacing);
    }

    if (L->dir != NULL)
    {
        drawConection(posX + 20, posY + 40, posX + displacement + 20, posY + verticalSpacing);
        drawTree(L->dir, level + 1, posX + displacement, posY + verticalSpacing, horizontalSpacing / 2, verticalSpacing);
    }

    // Passa o valor de balanceamento ao chamar drawNode
    drawNode(posX, posY, L->chave, L->bal);
}

void updateTreeDisplay(struct s_arvore *tree)
{
    int treeHeight = getHeight(tree->raiz);
    int horizontalSpacing = WIDTH / (1 << treeHeight);
    horizontalSpacing *= 3;

    gfx_clear();
    gfx_set_color(255, 165, 0);
    drawTree(tree->raiz, 0, WIDTH / 2, 50, horizontalSpacing, VERTICAL_SPACING_CONST);
    gfx_paint();
}