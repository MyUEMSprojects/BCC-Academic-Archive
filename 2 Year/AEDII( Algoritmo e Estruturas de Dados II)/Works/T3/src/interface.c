#include "interface.h"

int calculateHeight(struct s_no *L)
{
    if (L == NULL)
    {
        return 0;
    }

    int leftHeight = calculateHeight(L->esq);
    int rightHeight = calculateHeight(L->dir);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int calcularPosX(struct s_no *L, int level, int horizontalSpacing)
{
    if (L == NULL)
    {
        return 0;
    }

    int leftPosX = calcularPosX(L->esq, level + 1, horizontalSpacing);
    int rightPosX = calcularPosX(L->dir, level + 1, horizontalSpacing);

    if (L->esq == NULL && L->dir == NULL)
    {
        return horizontalSpacing / 2;
    }

    return (leftPosX + rightPosX) / 2;
}

void drawConection(int x1, int y1, int x2, int y2)
{
    gfx_set_color(255, 165, 0);
    gfx_line(x1, y1, x2, y2);
}

void drawNode(int x, int y, int key)
{
    int size = 40;

    gfx_set_color(255, 165, 0);
    gfx_filled_rectangle(x, y, x + size, y + size);

    char buffer[20];
    sprintf(buffer, "%d", key);
    gfx_set_color(0, 0, 0);
    gfx_text(x + 10, y + 10, buffer);

    gfx_set_color(0, 0, 0);
    gfx_rectangle(x, y, x + size, y + size);
}

void drawTree(struct s_no *L, int level, int posX, int posY, int horizontalSpacing, int verticalSpacing)
{
    if (L == NULL)
    {
        return;
    }

    // Ajuste o valor de horizontalSpacing conforme necessário
    int displacement = horizontalSpacing / 2;

    // Desenhar as conexões antes de desenhar os nós
    if (L->esq != NULL)
    {
        drawConection(posX + 20, posY + 40, posX - displacement + 20, posY + verticalSpacing);
        drawTree(L->esq, level + 1, posX - displacement, posY + verticalSpacing, horizontalSpacing, verticalSpacing);
    }

    if (L->dir != NULL)
    {
        drawConection(posX + 20, posY + 40, posX + displacement + 20, posY + verticalSpacing);
        drawTree(L->dir, level + 1, posX + displacement, posY + verticalSpacing, horizontalSpacing, verticalSpacing);
    }

    // Desenhar o nó após as conexões
    drawNode(posX, posY, L->chave);
}

void updateTreeDisplay(struct s_arvore *tree)
{
    int treeHeight = calculateHeight(tree->raiz);
    int horizontalSpacing = WIDTH / (1 << treeHeight);
    horizontalSpacing *= 4;

    gfx_clear();
    gfx_set_color(255, 165, 0);
    drawTree(tree->raiz, 0, WIDTH / 2, 50, horizontalSpacing, VERTICAL_SPACING_CONST);
    gfx_paint();
}