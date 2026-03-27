/////////////////////////////////////////////////////////////////////
// sine.cpp
//
// This program draws a sine curve with vertices equally apart
// along the x-axis. The larger the number of vertices the better
// the curve approximates the sine wave.
//
// Interaction:
// Press +/- to increase/decrease the number of vertices of the curve.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265358979324

using namespace std;

// Globals.
static int numVertices = 50; // Number of vertices to draw the sine curve
static float amplitude = 30.0; // Amplitude of sine wave
static float xOffset = 50.0; // X offset to center the curve
static float yOffset = 50.0; // Y offset to center the curve
static float xScale = 15.0;  // Scale factor for x axis

// Drawing routine.
void drawScene(void)
{
   float t; // Parameter along x-axis
   int i;

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   // Draw sine curve as a line strip
   glBegin(GL_LINE_STRIP);
      for(i = 0; i <= numVertices; ++i)
      {
         t = -PI + (2.0 * PI * i) / numVertices;
         glVertex3f(xOffset + xScale * t,
                   yOffset + amplitude * sin(t),
                   0.0);
      }
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
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
         exit(0);
         break;
      case '+':
         numVertices++;
         glutPostRedisplay();
         break;
      case '-':
         if (numVertices > 3) numVertices--;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press +/- to increase/decrease the number of vertices in the sine curve." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("sine.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;
}
