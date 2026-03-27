#include "gfx.h"
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep  */

int main()
{
	printf("Testando a biblioteca gfx...");

	gfx_init(400, 200, "Teste!");
	
	gfx_set_color(255, 0, 0);
	gfx_rectangle(50, 50, 300, 150);
	gfx_set_color(0, 0, 255);
	gfx_filled_ellipse(60, 60, 50, 40);
	gfx_set_color(0, 255, 0);
	gfx_text(80, 80, "Texto");
	
	gfx_paint();

	sleep(5);

	gfx_quit();

	return 0;
}
