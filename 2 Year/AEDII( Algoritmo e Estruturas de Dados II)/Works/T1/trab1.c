#include "gfx.h"
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep  */

#define CENTER 300
#define RADIUS 240
#define SIZE 200
#define MAX_LEVEL 10

//Nome: Felipe Echeverria Vilhalva RGM: 45611

void drawSquare(int topLeftX, int topLeftY, int size) {
	gfx_set_color(0, 0, 0);
	gfx_filled_rectangle(topLeftX, topLeftY, (topLeftX + size), (topLeftY + size));
	gfx_set_color(255, 255, 255);
	gfx_rectangle(topLeftX, topLeftY, topLeftX + size, topLeftY + size);
}

void recursiveSquare(int topLeftX, int topLeftY, int size, int contribuition, int level) {
	if(level < 0) {
       return;
	}

	int halfSize = size/2;
	int halfContribuition = contribuition/2;

	recursiveSquare(topLeftX + contribuition, topLeftY + size, halfSize, halfContribuition, level - 1);
	recursiveSquare(topLeftX + size, topLeftY + contribuition, halfSize, halfContribuition, level - 1);
	recursiveSquare(topLeftX + contribuition, topLeftY - (0.5 * size), halfSize, halfContribuition, level - 1);
	recursiveSquare(topLeftX - (0.5 * size), topLeftY + contribuition , halfSize, halfContribuition, level - 1);

	drawSquare(topLeftX, topLeftY, size);
}


void recursiveCircle(int topLeftX, int topLeftY, int radius, int level) {
    if (level < 0) {
       return;
	}

	int radiusError = (radius/2.8);

	recursiveCircle(topLeftX - radiusError, topLeftY - radiusError, radius/2, level - 1);
	recursiveCircle(topLeftX + radiusError, topLeftY - radiusError, radius/2, level - 1);
	recursiveCircle(topLeftX - radiusError, topLeftY + radiusError, radius/2, level - 1);
	recursiveCircle(topLeftX + radiusError, topLeftY + radiusError, radius/2, level - 1);

	gfx_ellipse(topLeftX, topLeftY, radius, radius);	
}

void interface() {

    int opcao = 0;
	int level = 0;
	
	do {
	  system("clear");
	  printf("\t1 - Desenhar circulo\n\t2 - Desenhar Retangulo\n");
      scanf("%d", &opcao);
	}while(opcao != 1 && opcao != 2);

	do{
	  system("clear");
      printf("Digite o nivel de recursividade: ");
	  scanf("%d", &level);
	}while(level > MAX_LEVEL);

	// chamada das funções:
	printf("Inicializando gfx...\n");
	gfx_init(600, 600, "FIGURAS - RECURSIVAS");

	if(opcao == 1){
		if(level >= 0 && level <= MAX_LEVEL) {
           recursiveCircle(CENTER, CENTER, RADIUS, level);
		}

	}else {
		if(level >= 0 && level <= MAX_LEVEL) {
           recursiveSquare(CENTER - 100, CENTER - 100, SIZE, 50, level);
		}
	}
}

int main()
{  
	int continuar = 0;

	do {
	  system("clear");
	  interface();
	  gfx_paint();
	  sleep(10);
	  gfx_quit();
	  printf("\nPressione 0 para fazer outros testes...\n");
	  scanf("%d", &continuar);
	}while(continuar == 0);

	return 0;
}