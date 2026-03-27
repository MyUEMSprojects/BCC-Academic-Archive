/* Drag.c is the pickdepth.c program (from redbook) (it was modified by Mercedes 29/8/2019)
 * Picking is demonstrated in this program.  In
 * rendering mode, three overlapping rectangles are
 * drawn.  When the left mouse button is pressed,
 * selection mode is entered with the picking matrix.
 * Rectangles which are drawn under the cursor position
 * are "picked."  Pay special attention to the depth
 * value range, which is returned.
 * Dragging is also supported for each picked rectangle.
  */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define BUFSIZE 512

static GLsizei width, height; // OpenGL window size.
static unsigned int closestName = 0, selected; // Name of closest hit.
int FirsClick=0;
typedef struct{
        float x;
        float y;
}Pnt;
GLuint selectBuf[BUFSIZE];

// Currently drag point.
Pnt DragPoint, InitPoint, Trans;

static float Rect[3][3][3] = {
	{{200.0, 200.0, 1.0}, {300.0, 300.0, 1.0},{1,1,0}},
	{{250.0, 250.0, 0.0}, {350.0, 350.0, 0.0},{1,0,1}},
	{{150.0, 250.0, -1.0}, {250.0, 350.0, -1.0},{0,1,1}}
};

Pnt distance (Pnt P1, Pnt P2){
      Pnt t;
      t.x=P2.x-P1.x;
      t.y=P2.y-P1.y;
      return(t);
}
void Rect3d(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b){
   glColor3f(r,g,b);
   glBegin(GL_QUADS);
   glVertex3f (x1,y1,z1);
   glVertex3f (x2,y1,(z1+z2)/2);
   glVertex3f (x2,y2,z2);
   glVertex3f (x1,y2,(z1+z2)/2);
   glEnd();
}
void init(void){
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   glDepthRange(0.0, 1.0);  /* The default z mapping */
}
/* The three rectangles are drawn.  In selection mode,
 * each rectangle is given the same name.  Note that
 * each rectangle is drawn with a different z value.
 */
void drawRects(GLenum mode){
   int i;
   for (i=0;i<3;i++){
      if (mode == GL_SELECT )
         glLoadName(i+1);
      if (selected==i+1) {
         /* The selected Rect3d is dragged the value Trans*/
         glPushMatrix();
         glTranslatef (Trans.x,Trans.y,0.0);
      }
      Rect3d(Rect[i][0][0],Rect[i][0][1],Rect[i][0][2],
      Rect[i][1][0],Rect[i][1][1],Rect[i][1][2],
      Rect[i][2][0],Rect[i][2][1],Rect[i][2][2]);
      if (selected==i+1)
         glPopMatrix();
   }
}
 /* Process hit buffer to find record with smallest min-z value.*/
void findClosestHit(int hits, unsigned int buffer[]){
   unsigned int *ptr, minZ;

   minZ= 0xffffffff; // 2^32 - 1
   ptr = buffer;
   closestName = 0;
   for (int i = 0; i < hits; i++){
      ptr++;
	  if (*ptr < minZ){
	     minZ = *ptr;
		 ptr += 2;
		 closestName = *ptr;
		 ptr++;
	  }
	  else ptr += 3;
   }
}
/*  pickRects() sets up selection mode, name stack,
 *  and projection matrix for picking.  Then the objects
 *  are drawn.
 */
void pickRects(int button, int state, int x, int y){
   GLint hits,i;
   GLint viewport[4];

   if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
      return;
   if (!FirsClick){
     glGetIntegerv(GL_VIEWPORT, viewport);

     glSelectBuffer(BUFSIZE, selectBuf);
     (void) glRenderMode(GL_SELECT);

     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     /*  create 5x5 pixel picking region near cursor location */
     gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);
     glOrtho(0.0, (float)width, 0.0, (float)height, -1.1, 1.1);

     glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
     glLoadIdentity();

     glInitNames();
     glPushName(0);

     drawRects(GL_SELECT);

     hits = glRenderMode(GL_RENDER);

   // Restore viewing volume of the resize routine and return to modelview mode.
     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
     glMatrixMode(GL_MODELVIEW);

   // Determine closest of the hit objects (if any).
     findClosestHit(hits, selectBuf);

   /* First click for picking */
   printf ("%d\n", hits);
    if (hits){
              InitPoint.x=DragPoint.x=x;
              InitPoint.y=DragPoint.y=height-y;
              selected=closestName;
              FirsClick=1;
              glutPostRedisplay();
              }

  }else{ /* Second click, dragging is finished and the new positions of Rect are updated */
         for (i=0;i<2;i++){
               Rect[selected-1][i][0]+=Trans.x;
               Rect[selected-1][i][1]+=Trans.y;
         }
         FirsClick=0;
         Trans.x=Trans.y=0.0;
         selected=0;
    }
   }

// Mouse motion callback routine.
void mousePassiveMotion(int x, int y){
   // Update the location of the DragPoint as the mouse moves with no button pressed.
   if (FirsClick){
      DragPoint.x=x;
      DragPoint.y= height - y;
      Trans=distance(InitPoint,DragPoint);
      glutPostRedisplay();
   }
}

void display(void){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   drawRects(GL_RENDER);
   glFlush();
}

void reshape(int w, int h){
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
// Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.1, 1.1);

   // Pass the size of the OpenGL window to globals.
   width = w;
   height = h;
//   glOrtho(0.0, 8.0, 0.0, 8.0, -0.5, 2.5);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y){
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

/*  Main Loop
 */
int main(int argc, char **argv){
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutMouseFunc(pickRects);
   glutKeyboardFunc(keyboard);
   glutPassiveMotionFunc(mousePassiveMotion);
   glutMainLoop();
   return 0;
}
