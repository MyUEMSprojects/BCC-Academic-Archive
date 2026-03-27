/////////////////////////////////
// box.cpp
//
// This program draws a wire box.
//
// Sumanta Guha.
/////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.6, 0.4, 0.2); // Cor marrom para a mesa
   glLoadIdentity();

   // Posicionamento da mesa
   glTranslatef(0.0, 0.0, -15.0);

   // Desenha o tampo da mesa (vista de cima)
   glBegin(GL_POLYGON);
      glVertex3f(-4.0, -2.0, 0.0);  // Vértice inferior esquerdo
      glVertex3f(4.0, -2.0, 0.0);   // Vértice inferior direito
      glVertex3f(4.0, 2.0, 0.0);    // Vértice superior direito
      glVertex3f(-4.0, 2.0, 0.0);   // Vértice superior esquerdo
   glEnd();

   // Desenha as bordas da mesa
   glColor3f(0.3, 0.2, 0.1); // Cor marrom mais escura para as bordas
   glBegin(GL_LINE_LOOP);
      glVertex3f(-4.0, -2.0, 0.0);
      glVertex3f(4.0, -2.0, 0.0);
      glVertex3f(4.0, 2.0, 0.0);
      glVertex3f(-4.0, 2.0, 0.0);
   glEnd();

   glFlush();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("box.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;
}

