#ifndef MENU_H
#define MENU_H

#include "gfx.h"
#include "arvore.h"
#include "interface.h"
#include <unistd.h> /* sleep  */
#include "globalVariables.h"

// Menu principal da aplicação
int mainMenu(int option, struct s_arvore **tree);

#endif /* MENU_H */