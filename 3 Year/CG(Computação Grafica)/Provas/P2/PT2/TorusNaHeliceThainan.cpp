//////////////////////////////////////////////////////////////////////
// rotatingHelix3.cpp
//
// This program, based on helix.cpp, animates a helix by rotating
// it around its axis using a timer function called by glutTimerFunc().
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//
//Sumanta Guha.
//////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static int isAnimate = 0; // Animated?
static int animationPeriod = 50; // Time interval between frames.
static float angle = -10*PI; // Angle of rotation.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float R = 20.0; // Radius of helix.

   float t; // Angle parameter along helix.

   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   glColor3f(0.0, 0.0, 0.0);
   glLoadIdentity();
   // Rotate scene.
   glTranslatef(0.0, 0.0, -60.0);
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   glPushMatrix();

   glColor3f(0.0, 0.0, 1.0);
   // The Trick: to align the axis of the helix along the y-axis prior to rotation
   // and then return it to its original location.
   glTranslatef(R * cos(angle), angle, R * sin(angle));
   glRotatef(angle*180/PI, 0.0, 1.0, 0.0);
   glutSolidTorus(1.0, 2.0, 20.0, 20.0);
   glColor3f(1.0, 1.0, 1.0);
   //glutSolidSphere(1.0, 10.0, 10.0);

   glPopMatrix();

   glColor3f(0.0, 0.0, 0.0);

   glBegin(GL_LINE_STRIP);
   for(t = -10 * PI; t <= 10 * PI; t += PI/20.0)
      glVertex3f(R * cos(t), t, R * sin(t));
   glEnd();

   glDisable(GL_DEPTH_TEST);

   glutSwapBuffers();
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
   glLoadIdentity();
}

// Routine to increase the rotation angle.
void increaseAngle(void)
{
   angle += 0.157;
   if (angle > 360.0) angle -= 360.0;

   glutPostRedisplay();
}

// Timer function for glutTimerFunc().
// The parameter someValue(=1) is passed to animate by glutTimerFunc(),
// but is not used.
void animate(int someValue)
{
   if (isAnimate) increaseAngle();

   // Note that glutTimerFunc() calls animate() *once* after the
   // specified msecs. Therefore, to make repeated calls, animate()
   // must call itself again with glutTimerFunc() as below.
   glutTimerFunc(animationPeriod, animate, 1);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
         glDisable(GL_DEPTH_TEST); // Disable depth testing.
         exit(0);
         break;
      case ' ':
         if(isAnimate) isAnimate = 0;
		   else isAnimate = 1;
            break;
         break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
         if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
      case 'Y':
         Yangle -= 5.0;
         if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
      case 'z':
         Zangle += 5.0;
         if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
      case 'Z':
         Zangle -= 5.0;
         if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod +=5;
   if( key == GLUT_KEY_UP)
      if (animationPeriod > 5) animationPeriod -=5;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
        << "Press the up/down arrow keys to speed up/slow down animation." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("rotatingHelix3.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   // Register the timer function animate().
   // The timer function is called after 5 msecs. with
   // the parameter value 1 passed to it.
   glutTimerFunc(5, animate, 1);

   glutMainLoop();

   return 0;
}
